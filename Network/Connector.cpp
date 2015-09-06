#include "Connector.h"

void * connect_thread(void * param)
{
	printf ( "--->connect_thread 0x%x \n", pthread_self() );
	
	Connector	* pClass = (Connector *) param;
	Session 	* pSession;
	
	while (true)
	{
		sem_wait( &pClass->m_semConnect );
		//printf("After sem_wait.Begin to handle ConnectingList \n");
		if ( pClass->m_bShutdown )
		{
			printf ( "\n connect_thread 0x%x exit \n", pthread_self() );
			pthread_exit (NULL);
		}
		
		while ( !pClass->m_pConnectingList->empty() ) 
		{
			if ( pClass->m_bShutdown )
			{
				printf ( "\n connect_thread 0x%x exit \n", pthread_self() );
				pthread_exit (NULL);
			}
			
			pClass->m_pConnectingList->Lock();
			pSession = pClass->m_pConnectingList->front();
			pClass->m_pConnectingList->pop_front();
			pClass->m_pConnectingList->Unlock();
			
			int err = connect( pSession->GetSocket(), (SOCKADDR*)( pSession->GetSockAddr() ),sizeof(SOCKADDR_IN));
			
			if (err == SOCKET_ERROR)
			{
				//printf ( "connect fail, errno = %d\n", errno );
				
				pClass->m_pSync->m_pConnectFailList->Lock();
				pClass->m_pSync->m_pConnectFailList->push_back( pSession );			
				pClass->m_pSync->m_pConnectFailList->Unlock();
			}
			else 
			{
				printf("connect success\n");
				SocketOpt::Nonblocking( pSession->GetSocket() );
				SocketOpt::DisableBuffering( pSession->GetSocket() );

				pClass->m_pSync->m_pConnectSuccessList->Lock();
				pClass->m_pSync->m_pConnectSuccessList->push_back( pSession );			
				pClass->m_pSync->m_pConnectSuccessList->Unlock();
			}
		}
	}
}

Connector::Connector(void) 
{
	m_pConnectingList	= NULL;
	m_bShutdown 		= FALSE;
	m_hThread 			= 0;
}

Connector::~Connector(void)
{
	if ( !m_bShutdown )
		Shutdown();
	
	if ( m_pConnectingList )
		delete m_pConnectingList;
}

void Connector::Init( SyncHandler * _pSync) 
{
	m_pSync = _pSync;
	
	if ( m_pConnectingList )
		delete m_pConnectingList;
	m_pConnectingList = new SessionList;
	
	if ( sem_init(&m_semConnect,0,0) != 0)
	{
		perror("Semaphore initialization failed");
	}
	pthread_create(&m_hThread, NULL, connect_thread, (void*)this );
}

void Connector::Connect( Session * pSession )
{
	m_pConnectingList->Lock();
	m_pConnectingList->push_back( pSession );			
	m_pConnectingList->Unlock();
	
	sem_post(&m_semConnect);
}

void Connector::Shutdown()
{
	m_bShutdown = TRUE;
	
	sem_post(&m_semConnect);
	
	pthread_cancel(m_hThread);
	
	pthread_join(m_hThread, NULL);
	
	sem_destroy(&m_semConnect);
}