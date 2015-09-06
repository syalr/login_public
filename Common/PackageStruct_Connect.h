#ifndef _PACKAGESTRUCT_CONNECT_H_INCLUDED_
#define _PACKAGESTRUCT_CONNECT_H_INCLUDED_

#include "PackageStruct.h"
#include "Protocol_Connect.h"

#pragma pack(push, 1)


/*
 * 	B, binary; 
 *	K, k:v;
 *  H, http;
 *  J, Json;
 */ 
struct MSG_ENTERSERVER_SYN : public MSG_BASE
{
	char EnterType[4];		 // 四种访问格式
	MSG_ENTERSERVER_SYN() 
	{
		memset( this, 0, sizeof(MSG_ENTERSERVER_SYN) );
		m_byCategory = Connect_Protocol;
		m_byProtocol = ClientType_SYN;
	}
};


/*
 *	AGENT_SERVER
 *	GAME_SERVER 
 *	DB_SERVER	
 *	LINE_SERVER 
 *	LOGIN_SERVER
 *	WORLD_SERVER
 *	TEMP_SERVER 
 */
struct MSG_CONNECTION_SYN : public MSG_BASE
{
	BYTE ServerType;		// 四种访问格式
	MSG_CONNECTION_SYN() 
	{
		memset( this, 0, sizeof(MSG_CONNECTION_SYN) );
		m_byCategory = Connect_Protocol;
		m_byProtocol = ServerType_SYN;
	}
};

/*
 *	心跳机制采用发包更新 Socket 连接时间
 */
struct MSG_HEARTBEAT_SYN : public MSG_BASE
{
	BYTE ServerType;	   // 四种访问格式
	MSG_HEARTBEAT_SYN() 
	{
		memset( this, 0, sizeof(MSG_HEARTBEAT_SYN) );
		m_byCategory = Connect_Protocol;
		m_byProtocol = Heartbeat_SYN;
	}
};


#pragma pack(pop)
#endif // _PACKAGESTRUCT_GA_H_INCLUDED_
