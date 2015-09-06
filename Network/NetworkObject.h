#ifndef _NETWORKOBJECT_H_
#define _NETWORKOBJECT_H_

#include <Utility.h>
#include <Network.h>

//-------------------------------------------------------------------------------------------------
/// NetworkObject
//	- fnCreateAcceptedObject 
//	- (OnAccept, OnDisconnect, OnRecv, OnConnect)
//-------------------------------------------------------------------------------------------------
class NetworkObject
{
	friend class Session;
	friend class SyncHandler;

public:
	NetworkObject();
	virtual ~NetworkObject();

	void			Disconnect( BOOL bGracefulDisconnect = TRUE );
	BOOL			Send( BYTE *pMsg, WORD wSize );
	BOOL			SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize );
	void			Redirect( NetworkObject *pNetworkObject );
	
//	SOCKADDR_IN*	GetSockAddr();

	WORD 			GetPort();	// PORT
	std::string		GetIP();

	void            NotPackageHeader() { m_bPackageHander = FALSE; }   // default TRUE;
	BOOL 			IsPackageHeader() { return m_bPackageHander; }

protected:
	virtual void	OnAccept( DWORD dwNetworkIndex ) {}
	virtual void	OnDisconnect() {};
	virtual	void	OnRecv( BYTE *pMsg, WORD wSize ) = 0;
	virtual void	OnConnect( BOOL bSuccess, DWORD dwNetworkIndex ) {}
	virtual void	OnLogString( char *pszLog ) {}

	inline void		SetSession( Session *pSession ) { m_pSession = pSession; }

	Session			*m_pSession;
	BOOL			m_bPackageHander;
};

#endif
	
	
