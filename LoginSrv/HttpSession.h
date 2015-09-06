#ifndef _HttpSession_H_INCLUDED_
#define _HttpSession_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "ServerSession.h"

class HttpSession : public ServerSession
{
public:
	HttpSession();
	virtual ~HttpSession();
	
	virtual void Init();
	virtual void Release();
	
protected:
	virtual void	OnRecv( BYTE *pMsg, WORD wSize );
	virtual void	OnLogString( char * pszLog );
};

#endif // _AGENTPLAYER_H_INCLUDED_
