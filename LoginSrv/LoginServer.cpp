#include "LoginServer.h"

NetworkObject * CreateServerSideAcceptedObject();
VOID DestroyServerSideAcceptedObject( NetworkObject *pNetworkObject );
VOID DestroyServerSideConnectedObject( NetworkObject *pNetworkObject );

NetworkObject * CreateClientSideAcceptedObject();
VOID DestroyClientSideAcceptedObject( NetworkObject * pNetworkObject );
VOID DestroyClientSideConnectedObject( NetworkObject * pNetworkObject );

LoginServer * g_pLoginServer 		= NULL;
DWORD LoginServer::m_dwClearDelay 	= 0xFFFFFFFF;

LoginServer::LoginServer(void)
{
	m_bShutdown		= FALSE;
	m_pIOCPServer 	= NULL;
	
	// Keyv Server
	//m_pKeyvServer   = NULL;
	
	// UserKey
	memset( m_pUserSession, 0x0, PORT_MAX);
	
	// Overtime Table
	m_lsOvertimeTable.clear();
}

LoginServer::~LoginServer(void)
{
	if ( m_pIOCPServer ) {
		delete m_pIOCPServer;
	}
}

BOOL LoginServer::Init()
{
	//m_lsOvertimeTable.reserve(999);
	
	LoginFactory::Instance()->Init();
	
	m_pIOCPServer = new IOCPServer;
	
	SYNCHANDLER_DESC desc[2];
	
	desc[0].dwSyncHandlerKey			= SERVER_SYNCHANDLER;
	desc[0].dwMaxAcceptSession			= 9;
	desc[0].dwMaxConnectSession			= 9;
	desc[0].dwSendBufferSize			= 2000000;
	desc[0].dwRecvBufferSize		 	= 2000000;
	desc[0].dwTimeOut					= 0;
	desc[0].dwNumberOfAcceptThreads		= 1; // Accept
	desc[0].dwNumberOfIoThreads			= 8;
	desc[0].dwNumberOfConnectThreads	= 0; // Login Server don't Active connect.
	desc[0].dwMaxPacketSize				= 60000; //4096
	desc[0].fnCreateAcceptedObject		= CreateServerSideAcceptedObject;
	desc[0].fnDestroyAcceptedObject		= DestroyServerSideAcceptedObject;
	desc[0].fnDestroyConnectedObject	= DestroyServerSideConnectedObject;

	desc[1].dwSyncHandlerKey 			= CLIENT_SYNCHANDLER;
	desc[1].dwMaxAcceptSession			= 700;
	desc[1].dwMaxConnectSession			= 0;
	desc[1].dwSendBufferSize			= 1024 * 60; //60000
	desc[1].dwRecvBufferSize			= 1024 * 60; //60000
	desc[1].dwTimeOut					= 0;
	desc[1].dwNumberOfAcceptThreads		= 1; // Accept
	desc[1].dwNumberOfIoThreads			= 8;
	desc[1].dwNumberOfConnectThreads	= 0; // Connect
	desc[1].dwMaxPacketSize				= 1024 * 10;	//4096
	desc[1].fnCreateAcceptedObject		= CreateClientSideAcceptedObject;
	desc[1].fnDestroyAcceptedObject		= DestroyClientSideAcceptedObject;
	desc[1].fnDestroyConnectedObject	= DestroyClientSideConnectedObject;

	if ( !m_pIOCPServer->Init(desc, 2) ) {
		return FALSE;
	}
		
	StartServerSideListen();
	StartClientSideListen();
	
	//m_pKeyvServer = LoginFactory::Instance()->AllocKeyvSession();
	//if ( m_pKeyvServer == NULL) {
	//	printf("[LoginFactory::Instance()->AllocKeyvSession] fail\n");
	//	return FALSE;
	//}
	
	m_pJsonServer = LoginFactory::Instance()->AllocJsonSession();
	if ( m_pJsonServer == NULL) {
		printf("[LoginFactory::Instance()->AllocJsonSession] fail\n");
		return FALSE;
	}
	
	return TRUE;	
}

void LoginServer::StartServerSideListen() 
{
	if( !m_pIOCPServer->IsListening( SERVER_SYNCHANDLER) )
	{
		if ( !m_pIOCPServer->StartListen(SERVER_SYNCHANDLER, "127.0.0.1", 7010) ) // Login Port 7010
		{
			return;
		}
	}
}

void LoginServer::StartClientSideListen()
{
	if ( !m_pIOCPServer->IsListening( CLIENT_SYNCHANDLER) ) 
	{
		if ( !m_pIOCPServer->StartListen(CLIENT_SYNCHANDLER, "", 1234) ) // Clinet Port 1234
		{
			return;
		}
	}
}

BOOL LoginServer::MaintainConnection()
{
	if ( m_bShutdown ) {
		return TRUE;
	}
}

BOOL LoginServer::Update( DWORD dwDeltaTick )
{
	if(m_pIOCPServer)
	{
		m_pIOCPServer->Update();
	}

	// MaintainConnection();
	if ( dwDeltaTick > m_dwClearOvertime ) 
	{
		// OvertimeClear(dwDeltaTick);
		m_dwClearOvertime = dwDeltaTick + m_dwClearDelay;
		//printf( "[LoginServer::Update %d = %d] \n", m_dwClearOvertime, dwDeltaTick);
	}
	
	return TRUE;
}

BOOL LoginServer::OvertimeClear( DWORD dwDeltaTick )
{
	printf(" [ LoginServer::OvertimeClear dwDeltaTick= %d ] \n", dwDeltaTick);
	printf(" [ LoginServer::OvertimeClear size = %d ] \n", m_lsOvertimeTable.size());
	
	WORD wIndex = 0;
	UserSession * pSession = NULL;
	for (int i = 0; i< m_lsOvertimeTable.size(); ++i) {	
		wIndex = m_lsOvertimeTable.front();
		if ( wIndex==0 ) {
			m_lsOvertimeTable.pop_front();
			continue;
		}		
		printf(" [ LoginServer::OvertimeClear wIndex = %d ] \n", wIndex);
		pSession = m_pUserSession[wIndex];
		if ( pSession != NULL ) {
			printf(" [ LoginServer::OvertimeClear pSession = %d ] \n", pSession);
			if ( pSession->Update( dwDeltaTick ) ) {
				pSession->CloseSession();
			}
		}
		m_lsOvertimeTable.pop_front();
	}
}

BOOL LoginServer::SendToAllServer( BYTE * pMsg, WORD wSize)
{
	printf("[LoginServer::SendToAllServer]\n");
	
	//if ( m_pKeyvServer ) {
	//	return m_pKeyvServer->Send( pMsg, wSize );
	//}
	
	if ( m_pJsonServer ) {
		return m_pJsonServer->Send( pMsg, wSize );
	}
	return FALSE;
}

//ServerSession * LoginServer::GetKeyvSession() const {
//	return m_pKeyvServer;
//}

ServerSession * LoginServer::GetJsonSession() const {
	return m_pJsonServer;
}

// User Server;
BOOL LoginServer::SendToClient( BYTE * pMsg, WORD wSize )
{
	MSG_BASE_FORWARD * pBase = (MSG_BASE_FORWARD *) pMsg;
	
	WORD wIndex = pBase->m_wParameter;
	if ( wIndex == 0 ) {
		return FALSE;	
	}
	
	UserSession * pSession = m_pUserSession[wIndex];
	if ( pSession != NULL ) {
		WORD sendSize =  wSize - sizeof(MSG_BASE_FORWARD);
		BYTE * sendMsg = (BYTE *) ( pBase);
		sendMsg += sizeof(MSG_BASE_FORWARD);
		pSession->Send(sendMsg, sendSize);
	}
}

BOOL LoginServer::SetUserSession(WORD wIndex, UserSession * pSession)
{
	printf(" [LoginServer::SetUserSession] \n");
	
	if ( wIndex == 0 ) {
		return FALSE;
	}
	
	m_pUserSession[wIndex] = pSession;
	
	printf(" [ LoginServer::SetUserSession pSession = %d ] \n", pSession);
			
	//if ( pSession != NULL ) {
	//	m_lsOvertimeTable.push_back(wIndex);
	//	for (int i = 0; i< m_lsOvertimeTable.size(); ++i)
	//		printf(" [ LoginServer::SetUserSession size = %d ] \n", m_lsOvertimeTable.size());		
	//}
	//
	//printf(" [LoginServer::SetUserSession wIndex = %d ] \n", wIndex);
	
	return TRUE;
}

///////////////////////////////////////////////////////////////
// 服务器端
NetworkObject * CreateServerSideAcceptedObject()
{
	printf("[LoginServer::CreateServerSideAcceptedObject]: Alloc TempServerSession.\n");
	TempServerSession * obj = LoginFactory::Instance()->AllocTempServerSession();
	if ( obj == NULL) {
		printf("\nLoginFactory::Instance()->AllocTempServerSession() Fail.\n");
		return NULL;
	}
	return (NetworkObject *)(obj);
}

VOID DestroyServerSideAcceptedObject( NetworkObject *pObjs )
{	
	printf("[LoginServer::DestroyServerSideAcceptedObject] Function\n");
}

VOID DestroyServerSideConnectedObject( NetworkObject *pNetworkObject )
{
	printf("[LoginServer::DestroyServerSideConnectedObject]: Not Used.\n");
}

// 客户端
NetworkObject * CreateClientSideAcceptedObject() {
	printf("[LoginServer::CreateClientSideAcceptedObject]: Alloc UserSession.\n");
	
	UserSession * obj = LoginFactory::Instance()->AllocUserSession();
	if ( obj == NULL) {
		printf("\nLoginFactory::Instance()->AllocTempUserSession() Fail.\n");
		return NULL;
	}
	
	obj->Init();
	return (NetworkObject *)(obj);
}

VOID DestroyClientSideAcceptedObject( NetworkObject * pObjs ) {
	printf("[LoginServer::DestroyClientSideAcceptedObject]: Function\n");
	UserSession * pSession = (UserSession *)pObjs;
	pSession->Release();
}

VOID DestroyClientSideConnectedObject( NetworkObject * pNetworkObject ) {
	printf("[LoginServer::DestroyClientSideConnectedObject]: Not Used.\n");
}
