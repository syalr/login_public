#include "DBConnectDir.h"

VOID CallBackMessage1( char * szMessage )
{
	printf("执行SQL出错:%s; \n", szMessage);
	// AddLogMsg(LOG_OUT, "执行SQL出错:%s", szMessage);
	// AddLogMsg(LOG_FILE, "SQL error:%s", szMessage);
}

DBConnect::DBConnect(void)
{
	m_pDBCInstance = new IMysql;
	Init();
}

DBConnect::~DBConnect(void)
{	
	delete m_pDBCInstance;
}

BOOL DBConnect::Init()
{
	
}

BOOL DBConnect::QueryDirect(QueryResult * pQuery) 
{
	return m_pDBCInstance->Execute( pQuery );
}

BOOL DBConnect::QueryLargeDataDirect(QueryResult * pQuery) 
{
	return m_pDBCInstance->ExecuteLargeData( pQuery );
}


/*
 *
 *  访问DB数据库
*/
db_passport Obj_db_passport;

db_passport::db_passport(void) 
{
	
}

db_passport::~db_passport(void) 
{
	
}

BOOL db_passport::Init()
{
	HyDatabaseDesc db_descNew;
	db_descNew.DataBaseModuleType			= DBCInterfaceType_MYSQL;
	db_descNew.dwQueryPoolSize				= 2000;
	db_descNew.wQueryProcessorNum			= 1;
	db_descNew.pszDatabaseIP				= "127.0.0.1";
	db_descNew.pszDatabaseName				= "db_passport";
	db_descNew.pszUserName					= "root";
	db_descNew.pszUserPassword				= "123qwe";
	db_descNew.fnErrorMessage				= CallBackMessage1;
	
	m_fnErrorMessage		= db_descNew.fnErrorMessage;
	if( !m_pDBCInstance->Init( db_descNew.pszDatabaseIP, db_descNew.pszDatabaseName, db_descNew.pszUserName, db_descNew.pszUserPassword, m_fnErrorMessage ) )	
		return FALSE;
	if( !m_pDBCInstance->Connect() )																	
		return FALSE;
	return TRUE;
}