#include "LoginServerSession.h"

LoginServerSession::LoginServerSession()
{
	m_userCount = 0;
}

LoginServerSession::~LoginServerSession()
{
}

void LoginServerSession::Init()
{
	ServerSession::Init();
}

void LoginServerSession::Release()
{
	ServerSession::Release();
}

void LoginServerSession::Update()
{
	ServerSession::Update();
}

void LoginServerSession::OnRecv(BYTE *pMsg, WORD wSize)
{
	printf("[LoginServerSession::OnRecv]\n");
	printf("call g_PacketHandler.ParsePacket_AL function.\n");
}

void LoginServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	printf("[LoginServer]: LoginServerSession::OnConnect\n");
	ServerSession::OnConnect( bSuccess, dwNetworkIndex );
	if ( bSuccess ) {
		printf("[LoginServerSession::OnConnect] success.\n");
		printf("Send Server Type.\n");
		ServerSession::SendServerType();
	}
	else {
		printf("[LoginServer]: LoginServerSession::OnConnect Fail\n");
	}
}

void LoginServerSession::OnDisconnect()
{
	printf("[LoginServerSession::OnDisconnect]\n");
	ServerSession::OnDisconnect();
}
	
void LoginServerSession::OnLogString( char * pszLog)
{
	
}
