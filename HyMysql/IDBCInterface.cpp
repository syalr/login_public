#include "IDBCInterface.h"

#pragma warning(disable: 4996)

IDBCInterface::IDBCInterface(void)
{
}

IDBCInterface::~IDBCInterface(void)
{
}

BOOL IDBCInterface::Init( char * pszIPAddress, char * pszDatabaseName, char * pszUserName, char * pszUserPassword, fnCallBackMessage fnErrorMessage ) 
{ 
	strncpy( m_pszIPAddress, pszIPAddress, MAX_IP_ADDRESS_LENGTH );
	strncpy( m_pszDatabaseName, pszDatabaseName, MAX_NAME_LENGTH );
	strncpy( m_pszUserName, pszUserName, MAX_NAME_LENGTH );
	strncpy( m_pszUserPassword, pszUserPassword, MAX_NAME_LENGTH );
	m_fnErrorMessage = fnErrorMessage;
	return TRUE; 
}