#ifndef _TempServerSession_H_INCLUDED_
#define _TempServerSession_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "ServerSession.h"

class TempServerSession : public ServerSession
{
public:
	TempServerSession();
	virtual ~TempServerSession();

	eSERVER_TYPE	GetServerType() {	return TEMP_SERVER;  }
	
public:
	virtual void    Clear();
	virtual void	OnRecv(BYTE *pMsg, WORD wSize);
	virtual void	OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual void	OnLogString( char * pszLog);
		
private:
	BOOL m_bFirst;
};

#endif // _AGENTPLAYER_H_INCLUDED_
