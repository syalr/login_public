#ifndef _JSON_SERVERSESSION_H_INCLUDED_
#define _JSON_SERVERSESSION_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>

#include "ServerSession.h"

class JsonServerSession : public ServerSession
{
public:

	JsonServerSession();
	virtual ~JsonServerSession();
	
	//eSERVER_TYPE    GetServerType() { return JSON_SERVER; }
	
	virtual void    Init();
	
protected:

	virtual void	OnRecv(BYTE *pMsg, WORD wSize);
	virtual void	OnDisconnect();
	virtual void	OnLogString( char * pszLog);
	
};

#endif // _JSON_SERVERSESSION_H_INCLUDED_
