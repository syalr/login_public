#include "DBServerSession.h"

#include "PacketHandler.h"

DBServerSession::DBServerSession()
{	
}

DBServerSession::~DBServerSession()
{
}

void DBServerSession::Init()
{
	ServerSession::Init();
}


void DBServerSession::Release()
{
	ServerSession::Release();
}

void DBServerSession::Update()
{
	ServerSession::Update();
}

void DBServerSession::OnRecv(BYTE *pMsg, WORD wSize)
{
	printf("[DBServerSession::OnRecv]\n");
	printf("call g_PacketHandler.ParsePacket_JD function.\n");
	g_PacketHandler.ParsePacket_JD( this, (MSG_BASE *)pMsg, wSize );
}

void DBServerSession::OnConnect( BOOL bSuccess, DWORD dwSessionIndex )
{
	ServerSession::OnConnect( bSuccess, dwSessionIndex );
	if ( bSuccess ) {
		printf("[DBServerSession::OnConnect] success.\n");
		printf("Send MSG_CONNECTION_SYN to Login.\n");
		this->SendConnectionSyn();
	}
	else
		printf("[LoginServer]: DBServerSession::OnConnect Fail\n");
}

void DBServerSession::SendConnectionSyn()
{
	MSG_CONNECTION_SYN msg;
	msg.ServerType = JSON_SERVER;
	Send( (BYTE *)&msg, sizeof(MSG_CONNECTION_SYN) );
}

void DBServerSession::OnDisconnect()
{
	printf("LoginServer : [DBServerSession::OnDisconnect]\n");
	ServerSession::OnDisconnect();
}

void DBServerSession::OnLogString( char * pszLog)
{
	
}
