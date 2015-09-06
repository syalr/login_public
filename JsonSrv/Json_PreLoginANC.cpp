#include <Utility.h>
#include <iostream>
#include <Common.h>
#include "Yond_json.h"
#include "Json_PreLoginANC.h"

using std::cout;
using std::endl;

Json_PreLoginANC::Json_PreLoginANC()
{
}


Json_PreLoginANC::~Json_PreLoginANC()
{
	
}

void Json_PreLoginANC::SetMsg(MSG_PRELOGIN_ANC * pMsg)
{
	if ( pMsg != NULL ) {
		memcpy( &msg, pMsg, sizeof(msg) );
	}
}

WORD Json_PreLoginANC::GetJson(char * szJson, WORD wSize )
{
	char buff[1024]  = {0};
	char format[256] = 	"{\"userid\":\"%d\","
						"\"userkey\":\"%s\","
						"\"sip\":\"%s\","
						"\"port\":\"%d\"}";
	
	sprintf( buff, format, msg.m_uiUserID, msg.m_byUserKey, msg.m_byIP, msg.m_dwPort);
	
	int len = strlen(buff);
	if ( len < wSize ) {
		memcpy(szJson, buff, len);
	}
	return len;
}

