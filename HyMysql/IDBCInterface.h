#ifndef __DBC_INTERFACE_H_INCLUDED__
#define __DBC_INTERFACE_H_INCLUDED__

#include <Utility.h>
#include <HyDatabaseDefine.h>
class QueryResult;

class IDBCInterface
{
public:
	enum
	{
		DB_CONNECT_TIMEOUT		= 3,
		MAX_IP_ADDRESS_LENGTH	= 64,
		MAX_NAME_LENGTH			= 64,
		MSG_BUF_SIZE			= 128,
	};
	
	IDBCInterface(void);
	virtual ~IDBCInterface(void);

	virtual void			Release() {}
	virtual BOOL			Init( char * pszIPAddress, char * pszDatabaseName, char * pszUserName, char * pszUserPassword, fnCallBackMessage fnErrorMessage );
	virtual BOOL			Connect()							{ return FALSE; }
	virtual void			Disconnect()						{}
	virtual BOOL			Execute( QueryResult * pQuery )			{ __UNUSED(pQuery); return FALSE; }
	virtual BOOL			ExecuteLargeData( QueryResult * pQuery )	{ __UNUSED(pQuery); return FALSE; }

protected:
	char					m_pszIPAddress[MAX_IP_ADDRESS_LENGTH];
	char					m_pszDatabaseName[MAX_NAME_LENGTH];
	char					m_pszUserName[MAX_NAME_LENGTH];
	char					m_pszUserPassword[MAX_NAME_LENGTH];
	char					m_pszDeviceName[MAX_NAME_LENGTH];

	fnCallBackMessage		m_fnErrorMessage;
};


#endif // __DBC_INTERFACE_H__
	
	
