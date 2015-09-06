#include "UserSession.h"
#include "LoginServer.h"
#include "LoginFactory.h"

DWORD UserSession::m_dwClientDelay = 0xFFFFFFFF;
DWORD UserSession::m_dwServerDelay = 0xFFFFFFFF;

UserSession::UserSession()
: m_bFirst(TRUE)
{	
	m_eUserType = UT_TEMP_USER;
}

UserSession::~UserSession()
{
}

BOOL UserSession::SendPacket(BYTE *pMsg, WORD wSize)
{
	return Send( (BYTE *)pMsg, wSize );
}

WORD UserSession::GetUserKey() const
{
	return m_wUserKey;
}

void  UserSession::SetUserKey(WORD dwKey)
{
	m_wUserKey = dwKey;
}

/************ protected *************/
void UserSession::Init()
{
	m_wUserKey 		= 0;
	m_bFirst 		= TRUE;
	
	DWORD dwCurrent = Session::GetTickCount();
	m_dwOvertime    = dwCurrent + UserSession::m_dwClientDelay;
	printf( "[UserSession::Init %d] \n", m_dwOvertime);
	
	this->NotPackageHeader();
}

BOOL UserSession::Update( DWORD dwDeltaTick )
{
	printf( "[UserSession::Update %d = %d] \n", dwDeltaTick, m_dwOvertime);
	
	// Count Down;
	if ( dwDeltaTick > m_dwOvertime ) {
		Disconnect(TRUE);
		return TRUE;
	}
	
	return FALSE;
}

void UserSession::CloseSession()
{
	printf(" [ UserSession::CloseSession ] \n");
	
	if ( m_pSession != NULL) {
		m_pSession->CloseSocket();
	}
	
	m_bFirst = TRUE;
	
}

void UserSession::Release()
{
	printf(" [ UserSession::Release ] \n");
	
	m_bFirst = TRUE;
	
	g_pLoginServer->SetUserSession( this->m_wUserKey, NULL); 
	
	LoginFactory::Instance()->FreeUserSession(this);
}

void UserSession::OnAccept( DWORD dwNetworkIndex )
{
	WORD PortKey = this->GetPort();
	if ( PortKey != 0 )
	{
		this->SetUserKey(PortKey);
		g_pLoginServer->SetUserSession(PortKey, this);
		LoginFactory::Instance()->FreeUserSession( this );
		return;
	}
}

void UserSession::OnDisconnect()
{
	printf("[UserSession::OnDisconnect]\n");
	NetworkObject::OnDisconnect();
}

void UserSession::OnRecv(BYTE *pMsg, WORD wSize)
{
	printf(">>>> [UserSession::OnRecv]\n");
	
	// Connected warning.
	if ( m_bFirst == FALSE ) {
		this->Release();
		return;
	}
	
	BYTE msgPlus[1024] = {0};
	
	// Connected warning.
	m_bFirst = FALSE;
	
	// Alloc Port 
	if ( m_wUserKey != 0 )
	{
		MSG_BASE_FORWARD xMsg;
		xMsg.m_wParameter = m_wUserKey;
		memcpy( msgPlus, &xMsg, sizeof(xMsg) );
		memcpy( msgPlus + sizeof(xMsg), pMsg, wSize );
		g_pLoginServer->SendToAllServer( msgPlus, wSize + sizeof(xMsg) );
	}
	
	m_dwOvertime = Session::GetTickCount() + UserSession::m_dwServerDelay;
	printf( "[UserSession::OnRecv %d = %d] \n", m_dwOvertime);
}

void UserSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	
}

void UserSession::OnLogString( char * pszLog)
{
	
}
