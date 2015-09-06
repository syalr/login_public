#include "LoginServerSession.h"
#include "PacketHandler.h"
#include <Public.h>
#include "Json_PreLoginREQ.h"

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

void LoginServerSession::OnRecv(BYTE * pMsg, WORD wSize)
{
	printf("[LoginServerSession::OnRecv]\n");
	
	char json_msgs[1024] = {0};
	MSG_BASE_FORWARD bMsg; 
	memcpy(json_msgs, pMsg + sizeof(bMsg), wSize - sizeof(bMsg)); 
	
	JsonParser js_pid;
	if ( js_pid.ParseJson(json_msgs) == -1) {
		return;   // Error do not a json format;
	}
	
	MSG_PRELOGIN_REQ x;
	DWORD pid = js_pid.GetProtocol();
	
	printf("[LoginServerSession::OnRecv pid = %d] \n ", pid);
	if (pid != 0) 
	{
		memcpy( pMsg, &pid, sizeof(pid) );
		
		g_PacketHandler.ParsePacket_LJ( 
				(ServerSession*)this, 
				(MSG_BASE*) pMsg, 
				wSize);
	}
}

void LoginServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	printf("[LoginServer]: LoginServerSession::OnConnect\n");
	ServerSession::OnConnect( bSuccess, dwNetworkIndex );
	if ( bSuccess ) {
		printf("[LoginServerSession::OnConnect] success.\n");
		printf("Send MSG_CONNECTION_SYN to Login.\n");
		this->SendConnectionSyn();
	}
	else {
		printf("[LoginServer]: LoginServerSession::OnConnect Fail\n");
	}
}

void LoginServerSession::SendConnectionSyn()
{
	MSG_CONNECTION_SYN msg;
	msg.ServerType = JSON_SERVER;
	Send( (BYTE *)&msg, sizeof(MSG_CONNECTION_SYN) );
}

void LoginServerSession::OnDisconnect()
{
	printf("[LoginServerSession::OnDisconnect]\n");
	ServerSession::OnDisconnect();
}
	
void LoginServerSession::OnLogString( char * pszLog)
{
	
}
