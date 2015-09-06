#ifndef _PACKAGESTRUCT_H_INCLUDED_
#define _PACKAGESTRUCT_H_INCLUDED_

#include <Common.h>
#pragma pack(push, 1)

struct MSG_BASE
{
	MSG_BASE()
	{
		memset(this, 0, sizeof(MSG_BASE));
	}
	
	void Init(WORD wCMDType, WORD wCMDID)
	{
		m_byCategory = wCMDType;
		m_byProtocol = wCMDID;
	}

	WORD	m_byCategory;
	WORD	m_byProtocol;	
};

struct MSG_SERVER_TYPE : public MSG_BASE
{
	BYTE	m_byServerType;
	BYTE	m_byServerId;
};

struct MSG_BASE_FORWARD : public MSG_BASE
{
	union 
	{
		DWORD	m_dwParameter;
		WORD	m_wParameter;
		BYTE	m_byParameter;
	};
	
	MSG_BASE_FORWARD()
	{
		m_dwParameter  = 0;
	}
};

struct MSG_DBPROXY_RESULT : public MSG_BASE
{
	VOID *	m_pData;
};

struct MSG_HEARTBEAT : public MSG_BASE
{

};

struct MSG_TEST_PACK : public MSG_BASE
{
	int nNumber;
	short nShort;
	float nFloat;
	char strChar[20];
	DWORD dwDword;
};

#pragma pack(pop)
#endif // _PACKAGE_STRUCT_H_
