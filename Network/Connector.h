#ifndef __CONNECTOR_H_INCLUDED_
#define __CONNECTOR_H_INCLUDED_

#include <Utility.h>
#include <Network.h>

class Connector
{
	friend void * connect_thread(void * param);
	
public:
	Connector(void);
	~Connector(void);
	
	void Init( SyncHandler * pSync );
	
	void Connect(Session * pSession);
	
	void Shutdown();
	
public:	
	SyncHandler * 	m_pSync;
	SessionList * 	m_pConnectingList;
	
	pthread_t 	m_hThread;
	BOOL 		m_bShutdown;
	sem_t 		m_semConnect;
};

#endif //  __CONNECTOR_H_INCLUDED_
