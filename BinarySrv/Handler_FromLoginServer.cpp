#include "Handler_FromLoginServer.h"


Handler_FromLoginServer::Handler_FromLoginServer() 
{
}

Handler_FromLoginServer::~Handler_FromLoginServer() 
{
}

HANDLER_IMPL( PreLogin_REQ )
{
	printf(">> PreLogin_REQ\n");
	
	//MSG_AL_PRELOGIN_REQ * pRecvMsg = (MSG_AL_PRELOGIN_REQ *)pMsg;
	
	//MSG_LD_LOGIN_REQ msg2;
	//msg2.m_dwParameter = pRecvMsg->m_dwParameter;
	//msg2.m_pNetObj = (NetworkObject *) pServerSession;
	
	//memcpy(msg2.m_byUsername, pRecvMsg->m_byUsername, sizeof(pRecvMsg->m_byUsername) );
	//memcpy(msg2.m_byPassword, pRecvMsg->m_byPassword, sizeof(pRecvMsg->m_byPassword) );
	
	//g_LoginServer->SendToDBServer( (BYTE *)&msg2, sizeof(MSG_LD_LOGIN_REQ) );
}
