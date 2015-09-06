#include "HyDatabase.h"
#include "IMysql.h"
#include "QueryResult.h"

#define ASSERT(a) assert(a)
 
#if defined(WIN32) || defined( WIN64 )
	#define atoi64	_atoi64
#else
	#define atoi64	atoll
#endif


IMysql::IMysql(void)
	:	m_pMySql ( NULL )
{
}

IMysql::~IMysql(void)
{
}

VOID IMysql::Release()
{
	Disconnect();
	if (m_pMySql != NULL)
		delete m_pMySql;
}


BOOL IMysql::SQLError( const char* pszErr, QueryResult * pQuery )
{
	char Buffer[512];
	if ( pQuery )
	{
		sprintf( Buffer, "%s, %s, %s \n", pQuery->GetQuery(), pszErr,  mysql_sqlstate(m_pMySql) );
		m_fnErrorMessage( Buffer );
	}
	else
	{
		sprintf( Buffer, "%s, %s \n", pszErr,  mysql_sqlstate(m_pMySql) );
		m_fnErrorMessage( Buffer );
	}

	return FALSE;
}

BOOL IMysql::Init( char * pszIPAddress, char * pszDatabaseName, char * pszUserName, char * pszUserPassword, fnCallBackMessage fnErrorMessage )
{
	IDBCInterface::Init( pszIPAddress, pszDatabaseName, pszUserName, pszUserPassword, fnErrorMessage );

	ASSERT(1);
	//ASSERT( m_pMySql != NULL );

	m_pMySql = new MYSQL;

	if ( ! mysql_init( m_pMySql )  )
	{
		SQLError("mysql_init fail!\n");
		return FALSE;
	}

	return TRUE;
}

VOID IMysql::Disconnect()
{
	mysql_close( m_pMySql );
}

BOOL IMysql::Connect()
{
	ASSERT( m_pMySql );

	if ( ! mysql_real_connect( m_pMySql, m_pszIPAddress, m_pszUserName, m_pszUserPassword, m_pszDatabaseName, 0, NULL, CLIENT_MULTI_STATEMENTS|CLIENT_MULTI_RESULTS/*CLIENT_LOCAL_FILES*/)  )
	{
		SQLError( mysql_error(m_pMySql) );
		return FALSE;
	}

	 mysql_set_character_set( m_pMySql, "utf8");

	return TRUE;
}

// 如果断开则重连
void IMysql::TryConnect(void)
{
	int nRet = mysql_ping(m_pMySql);
	if (0 != nRet)
	{
		Disconnect();
		Connect();
	}
}

BOOL IMysql::ExecuteLargeData( QueryResult * pQuery )
{
	// 如果断开则重连
	TryConnect();

	MYSQL_STMT	*stmt			= NULL;
	MYSQL_BIND	*ps_bind		= NULL;
	MYSQL_RES	*rs_metadata	= NULL;
	MYSQL_BIND  *rs_bind		= NULL;  /* for output buffers */
	MYSQL_FIELD *fields			= NULL;  /* for result set metadata */
	my_bool		is_null_value	= 0;

	LONG	lSize		= 0;
	SHORT	sSqlType	= 0;
	SHORT	sCType		= 0;
	SHORT	ioType		= 0;
	BYTE *	ptmpPtr		= NULL;
	int		status		= 0;
	ULONG	ulRow		= 0;
	ULONG*	pPtr		= NULL;
	ULONG 	uNumRows 	= 0;
	int		num_fields	= 0;

	pQuery->SetResultColNum( 0 );
	pQuery->SetResultRowNum( 0 );

	MY_CHARSET_INFO charsetInfo;
	 mysql_get_character_set_info(m_pMySql,&charsetInfo);

	stmt = mysql_stmt_init(m_pMySql);
	if ( stmt == NULL )
	{
		SQLError("mysql_stmt_init fail!\n");
		return FALSE;
	}

	status = mysql_stmt_prepare(stmt, pQuery->GetQuery(), (ULONG)strlen(pQuery->GetQuery()) );
	if ( status )
	{
		SQLError(mysql_stmt_error(stmt), pQuery);
		return FALSE;		
	}

	int param_count = mysql_stmt_param_count(stmt);
	ASSERT( pQuery->GetParameterNum() == param_count );

	if ( param_count > 0 )
	{
		ps_bind = new MYSQL_BIND [ param_count ];
		memset(ps_bind, 0, sizeof (MYSQL_BIND) * param_count);

		for( ULONG i = 0 ; i < pQuery->GetParameterNum() ; ++i )	
		{	
			pQuery->GetParameterInfo( i, ioType, ptmpPtr, lSize, sSqlType, sCType );	

			if ( sCType == MYSQL_TYPE_STRING )
				lSize = strlen((char*)ptmpPtr);

			ps_bind[i].buffer_type		= (enum enum_field_types)sCType;
			ps_bind[i].buffer			= (char*)ptmpPtr;
			ps_bind[i].buffer_length	= lSize;
			ps_bind[i].length			= 0;
			ps_bind[i].is_null			= 0;

			//if( ioType == SQL_PARAM_INPUT )
			//	;
			//else
			//	;
			
		}

		// bind parameters 
		status = mysql_stmt_bind_param(stmt, ps_bind);
		if ( status )
		{
			SQLError(mysql_stmt_error(stmt), pQuery);
			goto CLEANUPFAIL;	
		}

		// Send blob fields
		for( ULONG index = 0 ; index < pQuery->GetParameterNum() ; ++index )	
		{	
			pQuery->GetParameterInfo( index, ioType, ptmpPtr, lSize, sSqlType, sCType );	

			if( ioType == SQL_PARAM_INPUT && sSqlType == MYSQL_TYPE_BLOB )
			{
				int lRemainSize = lSize;
				int lPutSize = 0;

				for( ; lRemainSize > 0 ; lRemainSize -= lPutSize )
				{
					lPutSize = min( 600, lRemainSize );
					if ( mysql_stmt_send_long_data(stmt, index, (const char*)ptmpPtr, lPutSize) )
					{
						SQLError(mysql_stmt_error(stmt), pQuery);
						goto CLEANUPFAIL;	
					}
					
					ptmpPtr += lPutSize;
				}

			}
			
		}
	}

	/* Execute the query */
	status = mysql_stmt_execute(stmt);
	if ( status )
	{
		SQLError(mysql_stmt_error(stmt), pQuery);
		goto CLEANUPFAIL;	
	}

	/* the column count is > 0 if there is a result set */
	/* 0 if the result is only the final status packet */
	num_fields = mysql_stmt_field_count(stmt);

	pQuery->SetResultColNum(num_fields);
	if( pQuery->GetResultColNum() == 0 )
	{
		/// update, insert, delete 
		goto CLEANUPMIDOK;
	}
	else if( pQuery->GetResultColNum() > pQuery->GetMaxColNum() )
	{
		//
		pQuery->SetResultColNum( pQuery->GetMaxColNum() );
	}

	/* process results until there are no more */
	/* there is a result set to fetch */
	rs_metadata = mysql_stmt_result_metadata(stmt);
	fields = mysql_fetch_fields(rs_metadata);

	rs_bind = new MYSQL_BIND[num_fields];	
	
	/* Now buffer all results to client */
	if (mysql_stmt_store_result(stmt))
	{
		SQLError(mysql_stmt_error(stmt), pQuery);
		goto CLEANUPFAIL;	
	}

	uNumRows = mysql_stmt_num_rows(stmt);

	/* fetch and display result set rows */
	while (ulRow < uNumRows)
	{
		if( ulRow == pQuery->GetMaxRowNum() ) 
			break;

		//add by df
		pQuery->AllocData();

		/* set up and bind result set output buffers */
		memset(rs_bind, 0, sizeof (MYSQL_BIND) * num_fields);
		for (ULONG i = 0; i < pQuery->GetResultColNum(); ++i)
		{
			pQuery->GetColumnInfo( ulRow, i, pPtr, lSize, sCType);

			rs_bind[i].buffer_type		= (enum enum_field_types)sCType; //fields[i].type;
			rs_bind[i].buffer			= (char*)pPtr;
			rs_bind[i].buffer_length	= lSize;
			rs_bind[i].length			= pQuery->GetColumnLengthPtr(i);
			rs_bind[i].is_null			= 0;
		}

		status = mysql_stmt_bind_result(stmt, rs_bind);
		if ( status )
		{
			SQLError(mysql_stmt_error(stmt), pQuery);
			goto CLEANUPFAIL;
		}

		status = mysql_stmt_fetch(stmt);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		ulRow ++ ;
	}

	pQuery->SetAffectNum(ulRow);

CLEANUPMIDOK:
	if(rs_metadata)
		mysql_free_result(rs_metadata); /* free metadata */

	do
	{
		MYSQL_RES* pRes = mysql_store_result(m_pMySql);
		mysql_free_result(pRes);
	}while(0 == mysql_next_result(m_pMySql));
	
	if ( mysql_stmt_close(stmt) )
	{
		SQLError("mysql_stmt_close", pQuery);
		//SQLError(mysql_stmt_error(stmt),pQuery);
	}

	delete [] ps_bind;
	delete [] rs_bind;

	return TRUE;


CLEANUPFAIL:
	if ( rs_metadata )
		mysql_free_result(rs_metadata); /* free metadata */
	
	if ( stmt && mysql_stmt_close(stmt) )
		SQLError("mysql_stmt_close", pQuery);

	if ( ps_bind )
		delete [] ps_bind;
	
	if ( rs_bind )
		delete [] rs_bind;

	pQuery->SetAffectNum( 0 );
	pQuery->SetResultRowNum( 0 );
	pQuery->SetResultColNum( 0 );

	return FALSE;
}

BOOL IMysql::Execute( QueryResult * pQuery )
{
	// 如果断开则重连
	TryConnect();

//	return ExecuteLargeData( pQuery );
	MYSQL_RES * result = NULL;
	ULONG ulRow			= 0;
	ULONG * pPtr		= NULL;
	LONG csize			= 0;
	SHORT sType			= 0; 
	ULONG * pColLenPtr	= NULL;
	MYSQL_ROW row;
	
	pQuery->SetResultColNum( 0 );
	pQuery->SetResultRowNum( 0 );

	// 
	pQuery->InitData();
	//

	MY_CHARSET_INFO charsetInfo;
	mysql_get_character_set_info(m_pMySql,&charsetInfo);

	if ( mysql_real_query(m_pMySql, pQuery->GetQuery(), (ULONG)strlen( pQuery->GetQuery() ) ) ) 
	{   
		SQLError( mysql_error(m_pMySql), pQuery );
		return FALSE;
	}   

	// 获得查询结果
	do
	{
		result = mysql_store_result(m_pMySql);   
		if ( result == NULL )
		{
			//SQLError(mysql_error(m_pMySql), pQuery );
			pQuery->SetAffectNum( 0 );
			pQuery->SetResultRowNum( 0 );
			pQuery->SetResultColNum( 0 );
			break;
		}

		pQuery->SetResultColNum( mysql_num_fields(result) );
		pQuery->SetAffectNum(0);

		if( pQuery->GetResultColNum() == 0 )
		{
			/// update, insert, delete
			break;		
		}
		else if( pQuery->GetResultColNum() > pQuery->GetMaxColNum() )
		{
			pQuery->SetResultColNum( pQuery->GetMaxColNum() );
		}

		ASSERT( pQuery->GetMaxRowNum() != 0 );
		ASSERT( pQuery->GetMaxColNum() != 0 );

		while ( (row = mysql_fetch_row(result)) ) 
		{   
			//if( ulRow == pQuery->GetMaxRowNum() ) 
			//	break;

			pQuery->AllocData();
			//

			ULONG* lengths = mysql_fetch_lengths(result);

			for( ULONG i = 0 ; i < pQuery->GetResultColNum() ; ++i )
			{
				pColLenPtr = pQuery->GetColumnLengthPtr( i );
				*pColLenPtr = lengths[i];

				pQuery->GetColumnInfo( ulRow, i, pPtr, csize, sType);	
				SetField(row[i], sType, pPtr, csize, *pColLenPtr);
			}

			++ulRow;	
		}

		pQuery->SetResultRowNum( ulRow );
	}while(!mysql_next_result(m_pMySql));

	mysql_free_result( result );
	
	return TRUE;
}


BOOL IMysql::SetField( const char* pSrc, SHORT sType, ULONG* pDst, ULONG nDstLen, ULONG nSrcLen )
{
	//add by df
	if(NULL == pSrc)
		return false;
	//

	switch ( sType )
	{
	case MYSQL_TYPE_TINY: 
		*( (INT8*)pDst ) = atoi(pSrc);
		break;

	case MYSQL_TYPE_SHORT: 
		*( (INT16*)pDst ) = atoi(pSrc);
		break;

	case MYSQL_TYPE_LONG: 
		if(pSrc == NULL)
			*( (INT32*)pDst ) = 0;
		else
			*( (INT32*)pDst ) = atol(pSrc);
		break;

	case MYSQL_TYPE_LONGLONG: 
		*( (INT64*)pDst ) = atoi64(pSrc);
		break;

	case MYSQL_TYPE_FLOAT:
		*( (float*)pDst ) = (float)atof(pSrc);
		break;

	case MYSQL_TYPE_DOUBLE:
		*( (double*)pDst ) = atof(pSrc);
		break;

	case MYSQL_TYPE_STRING:
		strncpy((char*)pDst, pSrc, nDstLen);
		break;

	case MYSQL_TYPE_BLOB:
		memcpy(pDst, pSrc, min(nDstLen, nSrcLen));
		break;

	default :
		SQLError("Unknown field type!\n");
		return FALSE;
	}

	return TRUE;

}

