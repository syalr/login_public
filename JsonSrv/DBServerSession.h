#ifndef _DBSERVERSESSION_H_INCLUDED_
#define _DBSERVERSESSION_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "ServerSession.h"

class DBServerSession : public ServerSession
{
public:
	DBServerSession();
	virtual ~DBServerSession();
	
	eSERVER_TYPE    GetServerType() { return JSON_SERVER; }
	
	virtual void    Init();
	virtual void 	Release();
	virtual void 	Update();
	
	void SendConnectionSyn();
protected:
	virtual void	OnRecv(BYTE *pMsg, WORD wSize);
	virtual void 	OnConnect( BOOL bSuccess, DWORD dwSessionIndex );
	virtual void	OnDisconnect();	
	virtual void	OnLogString( char * pszLog);
};

#endif // _DBSERVERSESSION_H_INCLUDED_
