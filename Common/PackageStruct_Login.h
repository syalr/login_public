#ifndef _PACKAGESTRUCT_LOGIN_H_INCLUDED_
#define _PACKAGESTRUCT_LOGIN_H_INCLUDED_


#include "PackageStruct.h"
#include "CommStruct.h"
#include "Protocol_Login.h"


#pragma pack(push, 1)


// 预登陆
struct MSG_PRELOGIN_REQ : public MSG_BASE_FORWARD
{
	DWORD 	m_dwAccessID;
	BYTE 	m_byUsername[33];
	BYTE 	m_byPassword[33];
	BYTE    m_bySshKey[33];
	DWORD   m_dwGameID;
	BYTE    m_dwLoginType;
	
	MSG_PRELOGIN_REQ() 
	{
		memset( this, 0, sizeof(MSG_PRELOGIN_REQ) );
		
		m_byCategory = Login_Protocol;
		m_byProtocol = PreLogin_REQ;
		
		m_dwParameter = 0; // dwUserID
		m_byParameter = 0;
	}
};

// 预登陆 应答
struct MSG_PRELOGIN_ANC : public MSG_BASE_FORWARD
{
	DWORD 	m_uiUserID;
	BYTE 	m_byUserKey[CODE_KEY_LEN + 1];
	BYTE	m_byIP[IP_MAX_LEN]; 	//32
	DWORD	m_dwPort;
	
	MSG_PRELOGIN_ANC() 
	{
		memset( this, 0, sizeof(MSG_PRELOGIN_ANC) );
		
		m_byCategory 	= Login_Protocol; 
		m_byProtocol 	= PreLogin_ANC;
		m_dwParameter 	= 0;				// dwUserID
	}
};


// 预登陆 报错
struct MSG_PRELOGIN_NAK : public MSG_BASE_FORWARD
{
	WORD error;
	MSG_PRELOGIN_NAK() 
	{
		memset( this, 0, sizeof(MSG_PRELOGIN_NAK) );
		
		m_byCategory = Login_Protocol;
		m_byProtocol = PreLogin_NAK;
	}
};


#pragma pack(pop)
#endif // _PACKAGESTRUCT_GA_H_INCLUDED_
