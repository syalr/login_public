#ifndef _JsonSession_H_INCLUDED_
#define _JsonSession_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "ServerSession.h"

class JsonSession : public ServerSession
{
public:
	JsonSession();
	virtual ~JsonSession();
	
	virtual void Init();
	virtual void Release();
	
protected:
	virtual void	OnRecv( BYTE *pMsg, WORD wSize );
	virtual void	OnLogString( char * pszLog );
};

#endif // _JsonSession_H_INCLUDED_
