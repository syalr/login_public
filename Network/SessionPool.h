#ifndef _SESSIONPOOL_H_INCLUDED_
#define _SESSIONPOOL_H_INCLUDED_

#include <Utility.h>
#include <Network.h>

class SessionPool 
{
public:
	SessionPool( DWORD dwSize, DWORD dwSendBufferSize, DWORD dwRecvBufferSize, DWORD dwMaxPacketSize, DWORD dwTimeOutTick, DWORD dwIndexStart, BOOL bAcceptSocket );
	~SessionPool();

	Session*		Alloc();
	void			Free( Session* pSession );
	int				GetLength();

private:
	void			Create( DWORD dwSize, DWORD dwSendBufferSize, DWORD dwRecvBufferSize, DWORD dwTimeOutTick, DWORD dwMaxPacketSize, DWORD dwIndexStart, BOOL bAcceptSocket );

	DWORD			m_dwMaxSize;
	SessionList		*m_pList;
};

#endif
	
