#include "Handler_FromDBServer.h"
#include "JsonServer.h"
#include "MsgBuff.h"
#include "Json_PreLoginANC.h"

Handler_FromDBServer::Handler_FromDBServer() 
{
	
}

Handler_FromDBServer::~Handler_FromDBServer() 
{
	
}

HANDLER_IMPL( PreLogin_ANC )
{
	printf(">> PreLogin_ANC\n");
	
	MSG_PRELOGIN_ANC * pRecvMsg = (MSG_PRELOGIN_ANC *) pMsg;
	Json_PreLoginANC js_prologin;
	js_prologin.SetMsg( ( MSG_PRELOGIN_ANC* )pMsg );
	
	DWORD dwlen = 0;
	char cBuff[256] = {0};
	dwlen = js_prologin.GetJson(cBuff, 256);
	if ( dwlen != 0 ) {
		
		MSG_BASE_FORWARD sendLogin;
		sendLogin.m_wParameter = pRecvMsg->m_wParameter;
		
		printf ("Handler_FromDBServer::PreLogin_ANC m_wParameter %d = %d \n", sendLogin.m_wParameter, pRecvMsg->m_wParameter);
		
		memcpy( &sendLogin, pMsg, sizeof(sendLogin) );
		
		BYTE byBuff[1024] = {0};
		CMsgBuff msgbuff( byBuff, 1024 );
		msgbuff.Write( &sendLogin, sizeof(sendLogin) );
		msgbuff.Write( cBuff, dwlen);
		dwlen = msgbuff.GetWriteLen();
		g_pJsonServer->SendToLoginServer( (BYTE*)&byBuff, dwlen );
		
		printf("Success. PreLogin_ANC\n");
	}
}


HANDLER_IMPL( PreLogin_NAK )
{
	printf(">> PreLogin_NAK\n");
	
	MSG_PRELOGIN_NAK * pRecvMsg = (MSG_PRELOGIN_NAK *) pMsg;	
	//memcpy( &msg, pMsg, sizeof(msg) );
	char cBuff[256] = {0};
	sprintf(cBuff, "PreLogin_NAK Error Code : %d\n", pRecvMsg->error );
	
	DWORD dwlen = strlen(cBuff) + 1;
	if ( dwlen != 0 ) {
		
	
		MSG_BASE_FORWARD sendLogin;
		sendLogin.m_wParameter = pRecvMsg->m_wParameter;
		
		printf ("Handler_FromDBServer::PreLogin_NAK m_wParameter %d = %d \n", sendLogin.m_wParameter, pRecvMsg->m_wParameter);
		
		memcpy( &sendLogin, pMsg, sizeof(sendLogin) );
		
		BYTE byBuff[1024] = {0};
		CMsgBuff msgbuff( byBuff, 1024 );
		msgbuff.Write( &sendLogin, sizeof(sendLogin) );
		msgbuff.Write( cBuff, dwlen);
		dwlen = msgbuff.GetWriteLen();
		g_pJsonServer->SendToLoginServer( (BYTE*)&byBuff, dwlen );
	
		printf("Success. PreLogin_NAK\n");	
	}
	
}
