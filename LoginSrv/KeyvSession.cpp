#include "KeyvSession.h"
#include "LoginServer.h"

KeyvSession::KeyvSession()
{
}

KeyvSession::~KeyvSession()
{
}

void KeyvSession::Init()
{
}

void KeyvSession::Release()
{
}

void KeyvSession::OnRecv(BYTE * pMsg, WORD wSize)
{
	g_pLoginServer->SendToClient(pMsg, wSize);
}

void KeyvSession::OnLogString( char * pszLog)
{
	
}


