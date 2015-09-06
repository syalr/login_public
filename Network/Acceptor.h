#ifndef __ACCEPTOR_H_INCLUDED_
#define __ACCEPTOR_H_INCLUDED_

#include <Utility.h>
#include <Network.h>

class Acceptor
{
	friend void * accept_epoll_event_thread(void * param);
	friend void * accept_thread(void * param);

public:
	Acceptor(void);
	~Acceptor(void);
	
	void Init( SyncHandler * pSync );
	
	BOOL StartListen( const char *szIP, WORD wPort);

	void Shutdown();

	inline BOOL IsListening() { 
		return m_listenSocket != INVALID_SOCKET; 
	}

	inline SOCKET GetListenSocket() { 
		return m_listenSocket; 
	}
	
public:	
	SyncHandler * 	m_pSync;
	SOCKET 		m_listenSocket;
	SOCKADDR_IN 	m_sockaddr;
	
	int 		m_bShutdown;
	pthread_t 	m_hAcceptThread;
};

#endif //  __ACCEPTOR_H_INCLUDED_
