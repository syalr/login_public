#include "Handler_FromLoginServer.h"
#include "JsonServer.h"
#include "Json_PreLoginREQ.h"

Handler_FromLoginServer::Handler_FromLoginServer() 
{
}

Handler_FromLoginServer::~Handler_FromLoginServer() 
{
}

HANDLER_IMPL( PreLogin_REQ )
{
	printf(">> Handler_FromLoginServer::PreLogin_REQ \n");
	//printf(">> [ %s ] \n", pMsg );
	
	MSG_BASE_FORWARD * pRecvMsg = (MSG_BASE_FORWARD *)pMsg;
	printf(">>> wParameter = %d\n", pRecvMsg->m_wParameter );
	
	char json_msgs[1024] = {0};
	memcpy(json_msgs, ((BYTE*)pMsg) + sizeof(pRecvMsg), wSize - sizeof(pRecvMsg));
	
	Json_PreLoginREQ js_prologin;
	js_prologin.ParseJson( ( char* ) json_msgs);
	
	MSG_PRELOGIN_REQ sendDB;
	if ( js_prologin.GetMsg(&sendDB) != NULL ) {
		sendDB.m_wParameter = pRecvMsg->m_wParameter;
		g_pJsonServer->SendToDBServer( (BYTE*) &sendDB, sizeof(sendDB) );
	}
}
