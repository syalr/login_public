#ifndef _KeyvSession_H_INCLUDED_
#define _KeyvSession_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "ServerSession.h"

class KeyvSession : public ServerSession
{
public:
	KeyvSession();
	virtual ~KeyvSession();
	
	virtual void Init();
	virtual void Release();
	
protected:
	virtual void	OnRecv( BYTE *pMsg, WORD wSize );
	virtual void	OnLogString( char * pszLog );
};

#endif // _KeyvSession_H_INCLUDED_
