#include "Acceptor.h"

void * accept_thread(void * param)
{
	Acceptor	* pClass = (Acceptor *) param;
	Session 	* pSession;
	printf("--->accept_thread:0x%x\n", pthread_self());	
	
	while ( !pClass->m_bShutdown )
	{
		struct sockaddr_in addr;
		int len = sizeof(struct sockaddr_in);
		memset(&addr, 0, sizeof(struct sockaddr_in));	
	
		SOCKET sock = accept(pClass->m_listenSocket, (sockaddr *)&addr, (socklen_t*)&len );
		//SOCKET sock = accept(pClass->m_listenSocket, NULL, NULL );
		//printf("eMsg = %s\n",strerror(errno));
		if (sock == -1)
			continue;
	
		SocketOpt::Nonblocking( sock );
		SocketOpt::DisableBuffering( sock );
				
		Session * pSession = pClass->m_pSync->AllocAcceptSession();
		if (pSession == NULL) {
			continue;
		}
		
		pSession->SetSocket(sock);
		pSession->SetSockAddr(addr);
		
		printf ("client %s accepted. \n", inet_ntoa(addr.sin_addr) );	
			
		pClass->m_pSync->m_pAcceptedSessionList->Lock();
		pClass->m_pSync->m_pAcceptedSessionList->push_back( pSession );			
		pClass->m_pSync->m_pAcceptedSessionList->Unlock();
	}
	printf ( "accept_thread 0x%x exit \n", pthread_self() );
	pthread_exit(0);
}

Acceptor::Acceptor(void) 
{
	m_listenSocket	= INVALID_SOCKET;
	m_bShutdown		= 0;
	m_hAcceptThread = 0;
}

Acceptor::~Acceptor(void)
{
	Shutdown();
}

void Acceptor::Init( SyncHandler * _pSync) 
{
	m_pSync = _pSync;
}

BOOL Acceptor::StartListen( const char *szIP, WORD wPort )
{
	if ( m_listenSocket != INVALID_SOCKET ) {
		return FALSE;
	}
		
	m_listenSocket = socket(AF_INET, SOCK_STREAM, 0);

	if ( m_listenSocket == INVALID_SOCKET ) {
		printf ( "\n create socket fail \n" );		
		return FALSE;
	}
	
	SocketOpt::ReuseAddr(m_listenSocket);
	
	memset( &m_sockaddr, 0, sizeof(m_sockaddr) );
	
	m_sockaddr.sin_family		= AF_INET;
	m_sockaddr.sin_addr.s_addr	= ( szIP == NULL || strlen (szIP) == 0) ? htonl(INADDR_ANY) : inet_addr(szIP);
	m_sockaddr.sin_port 		= htons( wPort );
	
	int err = bind (m_listenSocket, (SOCKADDR *)&m_sockaddr, sizeof(m_sockaddr) );
	
	if ( err == SOCKET_ERROR )
	{
		SocketOpt::CloseSocket(m_listenSocket);
		printf ( "\n [Acceptor::CreateListenSocket] socket listen fail! \n" );	
		return FALSE;
	}

	err = listen( m_listenSocket, 5 );
	if ( err == SOCKET_ERROR )
	{
		SocketOpt::CloseSocket( m_listenSocket );
		printf("\n [Acceptor::CreateListenSocket socket listen fail!] \n");
		return  FALSE;
	}
	
	pthread_create( &m_hAcceptThread, NULL, accept_thread, (void*) this );
	
	return TRUE;
}

void Acceptor::Shutdown()
{
	if ( m_listenSocket != INVALID_SOCKET )
	{
		SocketOpt::CloseSocket(m_listenSocket);
		m_bShutdown = TRUE;
		pthread_cancel(m_hAcceptThread);
		pthread_join(m_hAcceptThread, NULL);
		printf ("accept_thread 0x%x exit \n", m_hAcceptThread);
	}
	m_listenSocket = INVALID_SOCKET;
	m_hAcceptThread = 0;
	m_bShutdown = 0;
}
