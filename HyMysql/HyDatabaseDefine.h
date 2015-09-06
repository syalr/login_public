#ifndef __HYDATABAES_DEFINE_H_INCLUDED__
#define __HYDATABAES_DEFINE_H_INCLUDED__

#include <Common.h>
#include <Utility.h>
#include <mysql.h>



#define __DISABLE_COPY(className)				\
	private:						\
		className( const className &);			\
		void operator = ( const className &)


#define __UNUSED(variable)	variable

#define MAX_COL_NUM			255

#ifndef SQL_PARAM_INPUT
	#define SQL_PARAM_INPUT                  1
	#define SQL_PARAM_INPUT_OUTPUT           2
	#define SQL_PARAM_OUTPUT                 4
#endif


enum eDBCInterfaceType
{
	DBCInterfaceType_DB				= 0x01,
	DBCInterfaceType_OLEDB			= 0x02,
	DBCInterfaceType_SQLLIB			= 0x03,
	DBCInterfaceType_MYSQL			= 0x04,
};


typedef unsigned char	UCHAR;
typedef long int		SDWORD;
//typedef memcpy		memcpyFast;
//typedef memset		memsetFast;
class QueryResult;
typedef QueryResult				Query;
typedef void			(*fnCallBackMessage)( char * szMessage );
typedef void			(*fnCallBackQueryResult)( DWORD dwIndex, QueryResult * pData );



struct HyDatabaseDesc
{
	eDBCInterfaceType		DataBaseModuleType;
	DWORD					dwQueryPoolSize;
	WORD					wQueryProcessorNum;	///
	char *					pszDatabaseIP;
	char *					pszDatabaseName;
	char *					pszUserName;
	char *					pszUserPassword;
	fnCallBackQueryResult	fnQueryResult;
	fnCallBackMessage		fnErrorMessage;
};


//=================================================================================================
/**
	@remarks
		QueryResult custom class	DEFINE
*/
//=================================================================================================
#define _INIT_PARAM_DATA()												virtual inline eQUERYTYPE		GetQueryType() { return eLargeQuery; }
#define _BEGIN_PARAM_DATA( _tagType, _tagPtr, _nParam )					virtual inline ULONG			GetParameterNum() { return _nParam; } \
																		virtual inline eQUERYTYPE		GetQueryType() { return eLargeQuery; } \
																		virtual inline VOID GetParameterInfo( ULONG ulParamIndex, SHORT & ioType, BYTE *& pParamPtr, LONG & lSize, SHORT & sqlType, SHORT & cType ) \
																		{ \
																			_tagType	* pTagPtr = _tagPtr; \
																			switch( ulParamIndex ) \
																			{
#define _BINDING_PARAM_PTR( _paramIndex, _paramPtr, _ioType )				case _paramIndex: { \
																				pParamPtr = (BYTE *)(pTagPtr->_paramPtr); \
																				lSize = sizeof(pTagPtr->_paramPtr); \
																				sqlType = _GET_DB_TYPE( pTagPtr->_paramPtr ); \
																				cType = _GetDB_C_Type( pTagPtr->_paramPtr ); } \
																				ioType = _ioType; \
																				return;
#define _BINDING_PARAM_PTR_WITH_SIZE( _paramIndex, _paramPtr, _paramSize, _ioType )	case _paramIndex: { \
																				pParamPtr = (BYTE *)(pTagPtr->_paramPtr); \
																				lSize = _paramSize; \
																				sqlType = _GET_DB_TYPE( pTagPtr->_paramPtr ); \
																				cType = _GetDB_C_Type( pTagPtr->_paramPtr ); } \
																				ioType = _ioType; \
																				return;
#define _BINDING_PARAM( _paramIndex, _param, _ioType )						case _paramIndex: { \
																				pParamPtr = (BYTE *)&(pTagPtr->_param); \
																				lSize = sizeof(pTagPtr->_param); \
																				sqlType = _GET_DB_TYPE( pTagPtr->_param ); \
																				cType = _GetDB_C_Type( pTagPtr->_param ); \
																				ioType = _ioType; \
																				} return;
#define _END_PARAM_DATA()													} \
																		return ; \
																		}

#define _INIT_BINDING_DATA()													virtual inline ULONG			GetMaxRowNum() { return 0; } \
																				virtual inline ULONG			GetMaxColNum() { return 0; } \
																				virtual inline BYTE *			GetBuffer( ULONG ulRow ) { return NULL; }
#define _BEGIN_BINDING_DATA( _tagType, col_arr, col_len_arr, nrow, ncol )		typedef	_tagType	TagType;	\
																				virtual inline ULONG			GetMaxRowNum() { return nrow; }	\
																				virtual inline ULONG			GetMaxColNum() { return ncol; }	\
																				virtual inline BYTE *			GetBuffer( ULONG ulRow ) { return (BYTE *)&col_arr[ulRow]; }	\
																				virtual inline ULONG *			GetColumnLengthPtr( ULONG col ) \
																				{ \
																					return &col_len_arr[col]; \
																				} \
																				virtual inline VOID				GetColumnInfo( ULONG row, ULONG col, ULONG *& pColPtr, LONG & lSize, SHORT & sColType ) \
																				{ \
																					TagType * pTag = &col_arr[0]; \
																					switch(col)	\
																					{
#define _BINDING_COLUMN_PTR( col_num, col_data_ptr )								case col_num: { \
																						pColPtr = (ULONG *)(pTag[row].col_data_ptr); \
																						lSize = sizeof(pTag[row].col_data_ptr); \
																						sColType = _GET_DB_TYPE( pTag[row].col_data_ptr ); } \
																						return ;
#define _BINDING_COLUMN( col_num, col_data )										case col_num: { \
																						pColPtr = (ULONG *)&(pTag[row].col_data); \
																						lSize = sizeof(pTag[row].col_data); \
																						sColType = _GET_DB_TYPE( pTag[row].col_data ); } \
																						return ;

#define _END_BINDING_DATA()															} \
																					return ; \
																				}




#define _DEFINE_DB_TYPE_FUNCTION( ctype, odbctype )							inline unsigned int _GET_DB_TYPE(ctype) \
																				{ \
																					return odbctype; \
																				}
		_DEFINE_DB_TYPE_FUNCTION(unsigned char*,		MYSQL_TYPE_BLOB);			// binary or varbinary
		_DEFINE_DB_TYPE_FUNCTION(const char*,			MYSQL_TYPE_STRING)
		_DEFINE_DB_TYPE_FUNCTION(signed char,			MYSQL_TYPE_TINY)
		_DEFINE_DB_TYPE_FUNCTION(unsigned char,			MYSQL_TYPE_TINY)

		_DEFINE_DB_TYPE_FUNCTION(INT64,					MYSQL_TYPE_LONGLONG)
		//_DEFINE_DB_TYPE_FUNCTION(UINT64,				MYSQL_TYPE_LONGLONG)

		_DEFINE_DB_TYPE_FUNCTION(short,					MYSQL_TYPE_SHORT)  
		_DEFINE_DB_TYPE_FUNCTION(unsigned short,		MYSQL_TYPE_SHORT)

		_DEFINE_DB_TYPE_FUNCTION(int,					MYSQL_TYPE_LONG)
		_DEFINE_DB_TYPE_FUNCTION(unsigned int,			MYSQL_TYPE_LONG)

		_DEFINE_DB_TYPE_FUNCTION(float,					MYSQL_TYPE_FLOAT)
		_DEFINE_DB_TYPE_FUNCTION(double,				MYSQL_TYPE_DOUBLE)
		//_DEFINE_DB_TYPE_FUNCTION(unsigned long,			MYSQL_TYPE_LONG)



#define _DEFINE_DB_C_TYPE_FUNCTION(ctype, odbc_c_type)						inline int _GetDB_C_Type(ctype) \
																				{ \
																					return odbc_c_type; \
																				}
		_DEFINE_DB_C_TYPE_FUNCTION(unsigned char*,		MYSQL_TYPE_STRING)			// binary or varbinary
		_DEFINE_DB_C_TYPE_FUNCTION(const char*,			MYSQL_TYPE_STRING)

		_DEFINE_DB_C_TYPE_FUNCTION(char,				MYSQL_TYPE_TINY)
		_DEFINE_DB_C_TYPE_FUNCTION(unsigned char,		MYSQL_TYPE_TINY)

		_DEFINE_DB_C_TYPE_FUNCTION(INT64,				MYSQL_TYPE_LONGLONG)
		_DEFINE_DB_C_TYPE_FUNCTION(UINT64,				MYSQL_TYPE_LONGLONG)

		_DEFINE_DB_C_TYPE_FUNCTION(short,				MYSQL_TYPE_SHORT)  
		_DEFINE_DB_C_TYPE_FUNCTION(unsigned short,		MYSQL_TYPE_SHORT)

		_DEFINE_DB_C_TYPE_FUNCTION(int,					MYSQL_TYPE_LONG)
		_DEFINE_DB_C_TYPE_FUNCTION(unsigned int,		MYSQL_TYPE_LONG)

		_DEFINE_DB_C_TYPE_FUNCTION(float,				MYSQL_TYPE_FLOAT)
		_DEFINE_DB_C_TYPE_FUNCTION(double,				MYSQL_TYPE_DOUBLE)


#endif //__HYDATABAES_DEFINE_H__
