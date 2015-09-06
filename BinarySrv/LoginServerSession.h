#ifndef _LOGIN_SERVER_SESSION_H_INCLUDED_
#define _LOGIN_SERVER_SESSION_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "ServerSession.h"

class LoginServerSession : public ServerSession
{
public:
	LoginServerSession();
	virtual ~LoginServerSession();
	
	eSERVER_TYPE    GetServerType() { return LOGIN_SERVER; }
	
	virtual void    Init();
	virtual void    Release();
	virtual void    Update();
	
public:
	virtual void	OnRecv( BYTE *pMsg, WORD wSize );
    virtual void    OnConnect( BOOL bSuccess, DWORD dwSessionIndex );
	virtual void	OnDisconnect();	
	virtual void	OnLogString( char * pszLog );
	
public:
	unsigned short m_userCount;
};

#endif // _LOGIN_SERVER_SESSION_H_INCLUDED_
