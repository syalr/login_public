#include "HyDatabase.h"

#pragma warning (push)
#pragma warning (disable: 4201)
#pragma warning (disable: 4121)

#include "HyDatabase.h"
#include "IDBCInterface.h"
#include "IMysql.h"
#include "QueryExecuteProcessor.h"
#include "QueryResult.h"

#pragma warning (pop)

#define SAFE_DELETE(p) { if ( p ) { delete ( p ); ( p ) = NULL; } }


HyDatabase::HyDatabase(void)

	:	m_ppDBCInstance ( NULL ),
		m_pThreadPool ( NULL ),
		m_fnErrorMessage ( NULL ),
		m_fnQueryResult ( NULL ),
		m_pQueryList ( NULL ),
		m_pQueryCS ( NULL ),
		m_pQueryResultCS ( NULL ),
		m_pQueryResultPushList ( NULL ),
		m_pQueryResultPopList ( NULL ),
		m_wQueryProcessorNum ( 0 ),
		m_bEndProcess( FALSE ),
		m_dwDummy (	0 )
		
{
}

HyDatabase::~HyDatabase(void)
{

}


VOID HyDatabase::Release()
{
	m_bEndProcess = TRUE;

	m_lockWakeup.Lock();
	m_condWakeup.Broadcast();
	m_lockWakeup.Unlock();

	SAFE_DELETE( m_pThreadPool );
	SAFE_DELETE( m_pQueryList );
	SAFE_DELETE( m_pQueryResultPushList );
	SAFE_DELETE( m_pQueryResultPopList );

	/*
	if( m_pQueryResultPushList )
	{
		ASSERT( m_pQueryResultPushList->size() == 0 );

		m_pQueryResultPushList->clear();
		delete m_pQueryResultPushList;
	}
	if( m_pQueryResultPopList )
	{
		ASSERT( m_pQueryResultPopList->size() == 0 );

		m_pQueryResultPopList->clear();
		delete m_pQueryResultPopList;
	}
	*/



	///
	//m_pDBCInstance->Disconnect();
	//m_pDBCInstance->Release();
	if( m_ppDBCInstance )
	{
		for( WORD i = 0 ; i < m_wQueryProcessorNum ; ++i )
		{
			delete m_ppDBCInstance[i];
			m_ppDBCInstance[i] = NULL;
		}
		delete [] m_ppDBCInstance;
		m_ppDBCInstance = NULL;
	}
	SAFE_DELETE( m_pQueryCS );
	SAFE_DELETE( m_pQueryResultCS );
}

BOOL HyDatabase::Initialize( HyDatabaseDesc & desc )
{
	ASSERT( !m_ppDBCInstance );

	m_fnErrorMessage		= desc.fnErrorMessage;
	m_fnQueryResult			= desc.fnQueryResult;
	m_wQueryProcessorNum	= desc.wQueryProcessorNum;

	switch( desc.DataBaseModuleType )
	{
	/*case DBCInterfaceType_ODBC:
		{
			m_ppDBCInstance = new IDBCInterface*[m_wQueryProcessorNum];
			for( WORD i = 0 ; i < m_wQueryProcessorNum ; ++i )
				m_ppDBCInstance[i] = new IODBC;
		}
		break;
	case DBCInterfaceType_OLEDB:
		{
			m_ppDBCInstance = new IDBCInterface*[m_wQueryProcessorNum];
			for( WORD i = 0 ; i < m_wQueryProcessorNum ; ++i )
				m_ppDBCInstance[i] = new IOLEDB;
		}
		break;
	case DBCInterfaceType_SQLLIB:
		{
			m_ppDBCInstance = new IDBCInterface*[m_wQueryProcessorNum];
			for( WORD i = 0 ; i < m_wQueryProcessorNum ; ++i )
				m_ppDBCInstance[i] = new ISQLLIB;
		}
		break;*/
	case DBCInterfaceType_MYSQL:
		{
			m_ppDBCInstance = new IDBCInterface*[m_wQueryProcessorNum];
			for( WORD i = 0 ; i < m_wQueryProcessorNum ; ++i )
				m_ppDBCInstance[i] = new IMysql;
		}
		break;

	default:
		{
			ASSERT( !"Not Exist DataBaseModuleType" );
			Release();
			return FALSE;
		}
	}

	for( WORD i = 0 ; i < m_wQueryProcessorNum ; ++i )
	{
		if( !m_ppDBCInstance[i]->Init( desc.pszDatabaseIP, desc.pszDatabaseName, desc.pszUserName, desc.pszUserPassword, m_fnErrorMessage ) )	return FALSE;
		if( !m_ppDBCInstance[i]->Connect() )																									return FALSE;
	}
	
	m_pQueryCS				= new Yond_mutex;
	m_pQueryResultCS		= new Yond_mutex;

	m_pQueryList			= new CLinkedlist<QueryResult *>( desc.dwQueryPoolSize );
	m_pQueryResultPushList  = new CLinkedlist<QueryResult *>( desc.dwQueryPoolSize );
	m_pQueryResultPopList  = new CLinkedlist<QueryResult *>( desc.dwQueryPoolSize );

	//m_pQueryResultPushList	= new QUERYRESULT_LIST;//( desc.dwQueryPoolSize );
	//m_pQueryResultPopList	= new QUERYRESULT_LIST;//( desc.dwQueryPoolSize );

	m_pThreadPool = new Yond_thread_pool;
	m_pThreadPool->SetAutoRelease( TRUE );
	Yond_thread * pThread = NULL;
	for( DWORD i = 0 ; i < desc.wQueryProcessorNum ; ++i )
	{
		pThread = new QueryExecuteProcessor( *this, m_ppDBCInstance[i] );
		m_pThreadPool->AddThread( pThread, TRUE );
	}

	m_pThreadPool->BatchStartThread();

	m_bEndProcess = FALSE;

	return TRUE;
}

BOOL HyDatabase::DBQuery( QueryResult * pQuery )
{
	ASSERT( pQuery );

	if( m_bEndProcess )
		return FALSE;

	m_pQueryCS->Lock();
	m_pQueryList->AddTail( pQuery );
	m_pQueryCS->Unlock();

	m_lockWakeup.Lock();
	m_condWakeup.Signal();
	m_lockWakeup.Unlock();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////
VOID HyDatabase::Update()
{
	ASSERT( this );

	/// DB 
	if( !m_pQueryResultPushList || (m_pQueryResultPushList && m_pQueryResultPushList->GetCount() == 0 ))
		return ;
	
	m_pQueryResultCS->Lock();

	SwitchQueryResultList();

	m_pQueryResultCS->Unlock();

	
	/*
	QUERYRESULT_ITER	it;
	for( it = m_pQueryResultPopList->begin() ; it != m_pQueryResultPopList->end() ; ++it )
	{
		pQResult = *it;
		m_fnQueryResult( pQResult->GetIndex(), pQResult );
	}
	m_pQueryResultPopList->clear();
	*/
	BOOL bInfiniteLoop = TRUE;
	while( bInfiniteLoop )
	{
		QueryResult* pQResult = m_pQueryResultPopList->DeleteHead();

		if (pQResult==NULL)
		{
			break;
		}

		m_fnQueryResult( pQResult->GetIndex(), pQResult );
	}
}
