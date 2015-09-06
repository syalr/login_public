#include "ServerSession.h"

ServerSession::ServerSession()
{	
}

ServerSession::~ServerSession()
{
}

void ServerSession::Clear()
{
	m_dwSessionIndex	  = 0;
	m_wConnectPort		  = 0;
	
	m_bForConnect		  = FALSE;
	m_bConnection		  = FALSE;
	m_bTryConnection	  = FALSE;
	
	m_dwLastHeartbeatTick = Session::GetTickCount();	
	m_strConnectIP.clear();
}

void ServerSession::Init()
{
	this->Clear();
}

void ServerSession::Release()
{
	m_strConnectIP.clear();
}

void ServerSession::Update()
{
	if ( this->IsForConnect() )
	{
		DWORD dwCurTick = Session::GetTickCount();
		if ( dwCurTick - m_dwLastHeartbeatTick > 10000) 
		{
			m_dwLastHeartbeatTick = dwCurTick;
			
			MSG_HEARTBEAT msg;
			msg.m_byCategory	= 0;
			//msg.m_byProtocol    = SERVERCOMMON_HEARTBEAT;
			Send( (BYTE*)&msg, sizeof(MSG_HEARTBEAT) );
		}
	}
}

void ServerSession::OnRecv(BYTE *pMsg, WORD wSize)
{
}

void ServerSession::OnConnect( BOOL bSucces, DWORD dwNetworkIndex )
{
	if ( bSucces )
	{
		SetForConnect( TRUE );
		SetSessionIndex( dwNetworkIndex );
		m_bConnection = TRUE;
	}
}

void ServerSession::OnAccept( DWORD dwNetworkIndex )
{
	SetForConnect( FALSE );
	SetSessionIndex( dwNetworkIndex );
	m_bConnection = TRUE;
}

void ServerSession::OnDisconnect()
{
	m_bConnection = TRUE;
}

void ServerSession::SendServerType()
{
	m_bConnection = TRUE;
	
	MSG_SERVER_TYPE 	msg;
	msg.m_byCategory 	= 0;
	msg.m_byProtocol 	= 0;
	msg.m_byServerType 	= this->GetServerType(); // 登陆服务器
	msg.m_byServerId 	= 1;
	
	Send( (BYTE *)&msg, sizeof(MSG_SERVER_TYPE) );
}

void ServerSession::SetAddr( char * pszIP, WORD wPort )
{
	m_strConnectIP = pszIP;
	m_wConnectPort = wPort;
}

BOOL ServerSession::TryToConnect()
{
	if ( m_strConnectIP.empty() ) {
		return FALSE;
	}
	
	if ( m_bTryConnection )
		return FALSE;
	
	m_bTryConnection = TRUE;
	return TRUE;
}

void ServerSession::OnLogString( char * pszLog )
{
	
}