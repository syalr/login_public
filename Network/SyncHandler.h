#ifndef __SyncHandler_H_INCLUDED_
#define __SyncHandler_H_INCLUDED_

#include <Utility.h>
#include <Network.h>

class SyncHandler
{
	friend void * epoll_thread(void * param);
	friend void * io_thread(void * param);
	friend void * send_thread(void * param);
	friend void * connect_thread(void * param);
	friend void * accept_thread(void * param);
	friend class Acceptor;
	
public:
	SyncHandler();
	~SyncHandler();
	
	BOOL 		Init( IOCPServer *pIOCPServer, LPSYNCHANDLER_DESC lpDesc );
	BOOL 		StartListen(const char *IP, WORD wPort);
	void 		Update();
	void        Shutdown();
	BOOL 		IsRunning() { return !m_bShutdown;}
	DWORD		Connect(NetworkObject * pNet, char *IP, WORD wPort);
	BOOL 		IsListening();

	inline 
	DWORD       GetNumberOfConnections() {	return m_numActiveSessions; }
	inline 
	DWORD       GetKey() {	return m_dwKey; }
	
	void 		AddIoEvent(struct epoll_event * pEvent);
	int 		ModEpollEvent(Session *pSession, DWORD nEvent);
	int 		AddEpollEvent(Session *pSession);
	void 		DelEpollEvent(Session *pSession);
	
private:
	Session * AllocAcceptSession();
	Session * AllocConnectSession();
	void FreeSession(Session * pSession);
	
	void ProcessConnectSuccessList();
	void ProcessConnectFailList();
	void ProcessAcceptedSessionList();
	void ProcessActiveSessionList();

	void KickDeadSession();
	void ProcessSend();
	void KickAllSession();
	
private:
	BOOL m_bShutdown;
	
	SessionPool * m_pAcceptSessionPool;
	SessionPool * m_pConnectSessionPool;
	
	Acceptor  * m_pAcceptor;
	Connector * m_pConnector;
	
	IOCPServer	* m_pIOCPServer;
	SessionList * m_pActiveSessionList;
	SessionList * m_pAcceptedSessionList;
	SessionList * m_pConnectSuccessList;
	SessionList * m_pConnectFailList;
	SessionList * m_pTemplateList;

	DWORD 		m_dwKey;
	int 		m_epoll;
	DWORD		m_numIoThreads;
	pthread_t	m_hIoThread[MAX_IO_WORKER_THREAD];
	pthread_t	m_hEpollThread;
	
	CircuitQueue<struct epoll_event> * m_pEvents;
	Yond_mutex 	m_lockEvents;
	Yond_cond	m_condEvents;
	
	DWORD m_dwMaxPacketSize;
	DWORD m_numActiveSessions;
	DWORD m_dwMaxAcceptSession;
	
	fnCallBackCreateAcceptedObject m_fnCreateAcceptedObject;
	fnCallBackDestroyAcceptedObject m_fnDestroyAcceptedObject;
	fnCallBackDestroyConnectedObject m_fnDestroyConnectedObject;
};

#endif //  __Yond_ini_INCLUDED_
