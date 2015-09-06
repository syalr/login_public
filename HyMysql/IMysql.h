#ifndef __IMYSQL_H_INCLUDED__
#define __IMYSQL_H_INCLUDED__

#include <Utility.h>
#include "IDBCInterface.h"

class QueryResult;

class IMysql :	public IDBCInterface
{
public:
	IMysql(void);
	virtual ~IMysql(void);

	virtual void			Release();
	virtual BOOL			Init( 
							char * pszIPAddress, 
							char * pszDatabaseName, 
							char * pszUserName, 
							char * pszUserPassword, 
							fnCallBackMessage fnErrorMessage );
							
	virtual BOOL			Connect();
	virtual void			Disconnect();
	virtual BOOL			Execute( QueryResult * pQuery );
	virtual BOOL			ExecuteLargeData( QueryResult * pQuery );

private:
	BOOL					SQLError(const char *pszErr, QueryResult * pQuery = NULL);	
	BOOL					SetField( const char* szData, SHORT sType, ULONG* pPtr, ULONG cSize, ULONG dSize );
	void					TryConnect(void);

	MYSQL*					m_pMySql;
};


#endif // __IMYSQL_H__
	
	
	
