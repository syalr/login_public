#include "JsonServer.h"
#include "InfoParser.h"

NetworkObject * CreateServerSideAcceptedObject();
VOID DestroyServerSideAcceptedObject( NetworkObject *pNetworkObject );
VOID DestroyServerSideConnectedObject( NetworkObject *pNetworkObject );

JsonServer * g_pJsonServer = NULL;

JsonServer::JsonServer(void)
{
	m_bShutdown = FALSE;
	m_pIOCPServer = NULL;
	
	m_pDBServerSession = NULL;
	m_pLoginServerSession = NULL;
}

JsonServer::~JsonServer(void)
{
	if ( m_pIOCPServer )
		delete m_pIOCPServer;
}

BOOL JsonServer::Init()
{
	JsonFactory::Instance()->Init();
	
	BOOL bRet = g_InfoParser.Init( "./ServerInfo.ini" );
	if ( !bRet ) {
		printf("Parse Server Info fail\n");
		return FALSE;
	}
	
	m_pIOCPServer = new IOCPServer;
	
	SYNCHANDLER_DESC desc[1];
	
	desc[0].dwSyncHandlerKey			= SERVER_SYNCHANDLER;
	desc[0].dwMaxAcceptSession			= 9;
	desc[0].dwMaxConnectSession			= 9;
	desc[0].dwSendBufferSize			= 2000000;
	desc[0].dwRecvBufferSize		 	= 2000000;
	desc[0].dwTimeOut					= 0;
	desc[0].dwNumberOfAcceptThreads		= 0; // Accept
	desc[0].dwNumberOfIoThreads			= 8; // 
	desc[0].dwNumberOfConnectThreads	= 1; // Connect
	desc[0].dwMaxPacketSize				= 60000; //4096
	desc[0].fnCreateAcceptedObject		= CreateServerSideAcceptedObject;
	desc[0].fnDestroyAcceptedObject		= DestroyServerSideAcceptedObject;
	desc[0].fnDestroyConnectedObject	= DestroyServerSideConnectedObject;

	if ( !m_pIOCPServer->Init(desc, 1) ) {
		return FALSE;
	}

	// Login Server
	m_pLoginServerSession = JsonFactory::Instance()->AllocLoginServerSession();
	if ( m_pLoginServerSession ) {
		SERVER_INFO info = g_InfoParser.GetServerInfo( LOGIN_SERVER );
		m_pLoginServerSession->SetAddr( info.m_strIp, info.m_dwPort ); // Login Port 7010
	}
	
	// DB Server
	m_pDBServerSession = JsonFactory::Instance()->AllocDBServerSession();
	if ( m_pDBServerSession ) {
		SERVER_INFO info = g_InfoParser.GetServerInfo( DB_SERVER );
		m_pDBServerSession->SetAddr( info.m_strIp, info.m_dwPort ); // DB Port 7030
	}	
	
	return TRUE;	
}

BOOL JsonServer::ConnectToServer( ServerSession * pSession, char * pszIP, WORD wPort )
{
	if (pSession == NULL) {
		return FALSE;
	}
	printf("IP = %s, Port = %d\n", pszIP, wPort);
	
	return m_pIOCPServer->Connect( SERVER_SYNCHANDLER, (NetworkObject *)pSession, pszIP, wPort );
}

BOOL JsonServer::MaintainConnection()
{
	if (m_bShutdown) {
		return TRUE;
	}

	if ( m_pLoginServerSession ) {
		if ( m_pLoginServerSession->TryToConnect() ) {
			ConnectToServer( m_pLoginServerSession, (char *)m_pLoginServerSession->GetConnnectIP().c_str(), m_pLoginServerSession->GetConnnectPort() );
		}
	}
	
	if ( m_pDBServerSession ) {
		if ( m_pDBServerSession->TryToConnect() ) {
			ConnectToServer( m_pDBServerSession, (char *)m_pDBServerSession->GetConnnectIP().c_str(), m_pDBServerSession->GetConnnectPort() );
		}
	}
}

BOOL JsonServer::Update( DWORD dwDeltaTick )
{
	if(m_pIOCPServer)
	{
		m_pIOCPServer->Update();
	}

	// Connect: DBSrv LoginSrv
	MaintainConnection();
	
	return TRUE;
}

BOOL JsonServer::SendToLoginServer( BYTE * pMsg, WORD wSize)
{
	printf("[LineServer::SendToLoginServer]\n");
	
	if ( m_pLoginServerSession ) {
		return m_pLoginServerSession->Send( pMsg, wSize );
	}
	return FALSE;
}

BOOL JsonServer::SendToDBServer(BYTE * pMsg, WORD wSize)
{
	printf("[LineServer::SendToDBServer]\n");
	
	if ( m_pDBServerSession != NULL ) {
		return m_pDBServerSession->Send( pMsg, wSize );
	}
	return FALSE;
}

/*************************************************/
NetworkObject * CreateServerSideAcceptedObject() {
	printf("[LineServer::CreateServerSideAcceptedObject]: Do nothing.\n");
}

VOID DestroyServerSideAcceptedObject( NetworkObject *pObjs ) {
	printf("[LineServer::DestroyServerSideAcceptedObject]: Do nothing.\n");
}

VOID DestroyServerSideConnectedObject( NetworkObject *pNetworkObject ) {
	printf("[LineServer::DestroyServerSideConnectedObject]: Not Used.\n");
}

