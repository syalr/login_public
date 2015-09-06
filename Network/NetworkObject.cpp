#include "Network.h"	
#include "NetworkObject.h"
#include "Session.h"

NetworkObject::NetworkObject()
{
	m_pSession = NULL;
	m_bPackageHander = TRUE; 
}

NetworkObject::~NetworkObject()
{
}

void NetworkObject::Disconnect( BOOL bGracefulDisconnect )
{
	if( m_pSession )
	{
		m_pSession->Disconnect( bGracefulDisconnect );
	}
}

BOOL NetworkObject::Send( BYTE *pMsg, WORD wSize )
{
	if( !m_pSession 
		|| m_pSession->HasDisconnectOrdered() 
		|| m_pSession->ShouldBeRemoved() ) 
		return FALSE;

	return m_pSession->Send( pMsg, wSize );
}

BOOL NetworkObject::SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize )
{
	if( !m_pSession 
		|| m_pSession->HasDisconnectOrdered() 
		|| m_pSession->ShouldBeRemoved() ) 
		return FALSE;

	return m_pSession->SendEx( dwNumberOfMessages, ppMsg, pwSize );
}

void NetworkObject::Redirect( NetworkObject *pNetworkObject )
{
	assert( pNetworkObject != NULL && "NULL Redirect"  );
	assert( m_pSession != NULL );

	m_pSession->BindNetworkObject( pNetworkObject );
}


std::string NetworkObject::GetIP()
{
	if( m_pSession )
	{
		return m_pSession->GetIP();
	}
	else
	{
		return "";
	}
}

/*	
SOCKADDR_IN* NetworkObject::GetSockAddr()
{
	if( m_pSession )
	{
		return m_pSession->GetSockAddr();
	}
	else
	{
		return NULL;
	}
}*/
	
WORD NetworkObject::GetPort()
{
	if( m_pSession ) {
		return ntohs( m_pSession->GetSockAddr()->sin_port );
	}
	else {
		return 0;
	}
}
