#include "DBServer.h"

NetworkObject * CreateServerSideAcceptedObject();
VOID DestroyServerSideAcceptedObject( NetworkObject *pNetworkObject );
VOID DestroyServerSideConnectedObject( NetworkObject *pNetworkObject );

DBServer * g_DBServer = NULL;

DBServer::DBServer(void)
{
	m_bShutdown = FALSE;
	m_pIOCPServer = NULL;
	m_pJsonServer = NULL;
}

DBServer::~DBServer(void)
{
	if ( m_pIOCPServer )
		delete m_pIOCPServer;
}

BOOL DBServer::Init()
{
	Obj_db_passport.Init();
	
	DBFactory::Instance()->Init();
	
	m_pIOCPServer = new IOCPServer;

	SYNCHANDLER_DESC desc[1];
	
	desc[0].dwSyncHandlerKey			= SERVER_SYNCHANDLER;
	desc[0].dwMaxAcceptSession			= 9;
	desc[0].dwMaxConnectSession			= 9;
	desc[0].dwSendBufferSize			= 2000000;
	desc[0].dwRecvBufferSize		 	= 2000000;
	desc[0].dwTimeOut				= 0;
	desc[0].dwNumberOfAcceptThreads			= 1;
	desc[0].dwNumberOfIoThreads			= 1;
	desc[0].dwNumberOfConnectThreads		= 0; // DB Server don't Active connect.
	desc[0].dwMaxPacketSize				= 60000; //4096
	desc[0].fnCreateAcceptedObject		= CreateServerSideAcceptedObject;
	desc[0].fnDestroyAcceptedObject		= DestroyServerSideAcceptedObject;
	desc[0].fnDestroyConnectedObject	= DestroyServerSideConnectedObject;

	if ( !m_pIOCPServer->Init(desc, 1) ) {
		return FALSE;
	}

	StartServerSideListen();

	m_pJsonServer = DBFactory::Instance()->AllocJsonServerSession();
	if ( m_pJsonServer == NULL) {
		printf("[DBFactory::Instance()->AllocJsonServerSession] fail\n");
		return FALSE;
	}
	
	return TRUE;	
}

void DBServer::StartServerSideListen() 
{
	if( !m_pIOCPServer->IsListening( SERVER_SYNCHANDLER) ) {
		
		if ( !m_pIOCPServer->StartListen(SERVER_SYNCHANDLER, "", 7030) ) 	// DB Port 7030
		{
			return;
		}
	}
}

BOOL DBServer::MaintainConnection()
{
	if (m_bShutdown) {
		return TRUE;
	}
}

BOOL DBServer::Update( DWORD dwDeltaTick )
{
	if(m_pIOCPServer)
	{
		m_pIOCPServer->Update();
	}

	// MaintainConnection();
	
	return TRUE;
}

BOOL DBServer::SendToJsonServer( BYTE * pMsg, WORD wSize)
{
	printf("[DBServer::SendToJsonServer]\n");
	if ( m_pJsonServer ) {
		return m_pJsonServer->Send( pMsg, wSize );
	}
	return FALSE;
}
	
ServerSession * DBServer::GetJsonServerSession() const
{
	return m_pJsonServer;
}

///////////////////////////////////////////////////////////////
NetworkObject * CreateServerSideAcceptedObject() {
	printf("[DBServer::CreateServerSideAcceptedObject]: Alloc TempServerSession.\n");
	TempServerSession * pObj = DBFactory::Instance()->AllocTempServerSession();
	if ( pObj == NULL) {
		printf("\nDBServer::Instance()->AllocTempServerSession() Fail.\n");
		return NULL;
	}
	return (NetworkObject *)(pObj);
}

VOID DestroyServerSideAcceptedObject( NetworkObject *pObjs ) {
	printf("[DBServer::DestroyServerSideAcceptedObject] Function\n");
	
	#if 0
	ServerSession * pSession = (ServerSession *)pObjs;
	eSERVER_TYPE eType = pSession->GetServerType();
	
	if ( eType == LINE_SERVER ) {
		printf(">>>FreeLoginServerSession()\n");
		LineServerSession * obj = (LineServerSession *)pObjs;
		DBFactory::Instance()->FreeLineServerSession(obj);
	}
	#endif 
}

VOID DestroyServerSideConnectedObject( NetworkObject *pNetworkObject ) {
	printf("[DBServer::DestroyServerSideConnectedObject]: Not Used.\n");
}
