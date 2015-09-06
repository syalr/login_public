#include "Handler_FromJsonServer.h"

#include <Public.h>

#include "DBServer.h"

#include "PreLoginQuery.h"

Handler_FromJsonServer::Handler_FromJsonServer() 
{
	
}

Handler_FromJsonServer::~Handler_FromJsonServer() 
{
	
}

HANDLER_IMPL( PreLogin_REQ )
{
	printf(">>> PreLogin_REQ\n");
	
	MSG_PRELOGIN_REQ * pRecvMsg = (MSG_PRELOGIN_REQ *)pMsg;
	// MSG_PRELOGIN_REQ 消息包的 m_dwAccessID 没用到
	printf("UserName: %s, Password:%s \n", pRecvMsg->m_byUsername, pRecvMsg->m_byPassword );
	//pRecvMsg->
	// 生成 用户 MD5s( 随机数 + lastime + rid )
	char szUserSshKey[ CODE_KEY_LEN + 1 ] = { 0 }; // 33位
	
	char szRandom[11] = {0}; // 生成一个5位的随机值
	if ( is_support_drng() ) {
		// 支持随机数
		GetRandom(szRandom);				
	}
	else {
		// 不支持随机数
		GetRandom_C(szRandom);			
	}
	printf("Random = %s\n", szRandom);
	// MD5加密			
	string tmpStr(szRandom);
	//tmpStr += ; // 获取当前时间
	string outMD5 = MD5(tmpStr).toString() ;	
	sprintf( (char *)szUserSshKey, "%s", outMD5.c_str() );
	szUserSshKey[ CODE_KEY_LEN ] = '\0';
	printf("User SSH Key = %s\n", szUserSshKey );
	
	// 查询数据库
	CHAR szQueryBuff[1024];
	snprintf( szQueryBuff, sizeof(szQueryBuff), "call p_PreLogin(\'%s\',\'%s\',\'%s\',\'%s\', %d, %d)", 
						pRecvMsg->m_byUsername,
						pRecvMsg->m_byPassword,
						pRecvMsg->m_bySshKey,
						szUserSshKey,
						pRecvMsg->m_dwGameID,
						pRecvMsg->m_dwLoginType
						);
	Query_PreLogin * pQuery = Query_PreLogin::ALLOC();
		
	if ( NULL != pQuery ) {
		pQuery->SetQuery( szQueryBuff );
		Obj_db_passport.QueryDirect( pQuery );
		
		int nRet = pQuery->vctRes[0].m_nErrCode;
		if ( nRet == 0 ) {
			printf ( "UserID = %d, Port = %d, IP = %s \n", pQuery->vctRes[0].m_uiUserID, 
														   pQuery->vctRes[0].m_uiPort,
														   pQuery->vctRes[0].m_szIP
					);
			
			// 返回 PreLogin 应答消息
			MSG_PRELOGIN_ANC msg2;
			msg2.m_wParameter  = pRecvMsg->m_wParameter;
			printf ("Handler_FromJsonServer::PreLogin_REQ m_wParameter %d = %d \n", msg2.m_wParameter, pRecvMsg->m_wParameter);
			msg2.m_uiUserID 	= pQuery->vctRes[0].m_uiUserID; // User ID
			msg2.m_dwPort 		= pQuery->vctRes[0].m_uiPort; 	// Port			
			memcpy( msg2.m_byIP, pQuery->vctRes[0].m_szIP, IP_MAX_LEN ); 	// IP 
			memcpy( msg2.m_byUserKey, szUserSshKey, CODE_KEY_LEN + 1 ); 	// User SSH Key
			
			g_DBServer->SendToJsonServer( (BYTE *)&msg2, sizeof(msg2) );
			
			printf("Success. PreLogin_REQ\n");
		}
		else {
			printf("Handler_FromJsonServer::PreLogin_REQ ErrorCode : %d \n",nRet);
			// 返回出错消息 MSG_PRELOGIN_NAK
			MSG_PRELOGIN_NAK msg2;
			msg2.m_wParameter  = pRecvMsg->m_wParameter;
			printf ("Handler_FromJsonServer::PreLogin_REQ m_wParameter = %d \n", pRecvMsg->m_wParameter);
			msg2.error = nRet;
			
			g_DBServer->SendToJsonServer( (BYTE *)&msg2, sizeof(msg2) );
		}	
	
		Query_PreLogin::FREE( pQuery );
		pQuery = NULL;
		
		
	}
	
	
}
