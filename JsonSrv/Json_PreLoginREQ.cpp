#include <Utility.h>
#include <iostream>
#include <Common.h>
#include "Yond_json.h"
#include "Json_PreLoginREQ.h"

using std::cout;
using std::endl;

Json_PreLoginREQ::Json_PreLoginREQ()
{
	dwStatus = 0;
}


Json_PreLoginREQ::~Json_PreLoginREQ()
{
	
}

int Json_PreLoginREQ::ParseJson( const char * pInput )
{
	dwStatus = 0;
	
	cJSON * root = NULL;
	
	root = cJSON_Parse( pInput );
	if ( root == NULL ) {
		return -1;
	}
	
	int nLen = 0;
	
	// access id
	cJSON * accessid = cJSON_GetObjectItem( root, "aid" );
	if ( accessid ) {
		dwStatus |= eACCESSID;
		msg.m_dwAccessID = accessid->valueint;
	}
	
	// UserName
	cJSON * username = cJSON_GetObjectItem( root, "UserName" );
	if ( username ) {
		dwStatus |= eUSERNAME;
		nLen = strlen( username->valuestring );
		if ( nLen < sizeof(msg.m_byUsername) )
		{
			memcpy( msg.m_byUsername, username->valuestring, nLen + 1 );
			msg.m_byUsername[ nLen ] = '\0';
		}
	}
	
	// Password
	cJSON * pwd = cJSON_GetObjectItem( root, "Password" );
	if ( pwd ) {
		dwStatus |= ePASSWORD;
		nLen = strlen( pwd->valuestring );
		if ( nLen < sizeof(msg.m_byPassword) )
		{
			memcpy( msg.m_byPassword, pwd->valuestring, nLen + 1 );
			msg.m_byPassword[ nLen ] = '\0';
		}
	}
	
	// SshKey
	cJSON * sshkey = cJSON_GetObjectItem( root, "sshkey" );
	if ( sshkey ) {
		dwStatus |= eSSHKEY;
		nLen = strlen( sshkey->valuestring );
		if ( nLen < sizeof(msg.m_bySshKey) )
		{
			memcpy( msg.m_bySshKey, sshkey->valuestring, nLen + 1 );
			msg.m_bySshKey[ nLen ] = '\0';
		}
	}
	
	// game id
	cJSON * gameid = cJSON_GetObjectItem( root, "gameid" );
	if ( gameid ) {
		dwStatus |= eGAMEID;
		msg.m_dwGameID = gameid->valueint;
	}
		
	if ( root ) {
		cJSON_Delete( root );
	}
	
	return 0;
}
	
MSG_PRELOGIN_REQ * Json_PreLoginREQ::GetMsg(MSG_PRELOGIN_REQ * pMsg)
{
	if ( dwStatus & 0x48)
	{	
		msg.m_dwLoginType = JSON_SERVER;
		memcpy( pMsg, &msg, sizeof(msg) );
		return pMsg;
	}
	return NULL;
}

