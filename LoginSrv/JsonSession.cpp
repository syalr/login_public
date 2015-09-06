#include "JsonSession.h"
#include "LoginServer.h"

JsonSession::JsonSession()
{	
}

JsonSession::~JsonSession()
{
}

void JsonSession::Init()
{
	// Test
	//printf("[ JsonSession::Init ]");
	//
	//char msgbuff[1024] = "                        {\"Protocol\" : \"13107900\", \"aid\" : \"1\", \"sshkey\" : \"sad1\",  \"UserName\" : \"kim\",  \"Password\" : \"123qwe\", \"gameid\" : \"123\" }";
	//MSG_BASE_FORWARD bMsg;
	//this->Send( (BYTE*) msgbuff, strlen(msgbuff) );
}

void JsonSession::Release()
{
	
}

void JsonSession::OnRecv(BYTE *pMsg, WORD wSize)
{
	printf("JsonSession::OnRecv\n");
	MSG_BASE_FORWARD * pRecvMsg = (MSG_BASE_FORWARD *)pMsg;
	printf("m_wParameter : %d\n", pRecvMsg->m_wParameter);
	
	g_pLoginServer->SendToClient(pMsg, wSize);
}

void JsonSession::OnLogString( char * pszLog)
{
	
}
	