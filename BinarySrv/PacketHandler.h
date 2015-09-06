#ifndef _PACKET_HANDLER_H_INCLUDED_
#define _PACKET_HANDLER_H_INCLUDED_

#include <Common.h>
#include <Network.h>
#include <Utility.h>
using namespace A;

#include "ServerSession.h"

typedef VOID (*fnHandler)( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

class PacketHandler
{
public:
	PacketHandler(void);
	~PacketHandler(void);

public:
	BOOL RegisterHandler();
	void Register_LB();
	void Register_BD();
	
	BOOL AddHandler_LB( WORD category, WORD protocol, fnHandler fnHandler);
	BOOL AddHandler_BD( WORD category, WORD protocol, fnHandler fnHandler);

	void ParsePacket_LB( ServerSession * pUserSession, MSG_BASE * pMsg, WORD wSize );
	void ParsePacket_BD( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

private:
	struct FUNC_LB : public BASE_FUNC
	{
		fnHandler	m_fnHandler;
	};

	struct FUNC_BD : public BASE_FUNC
	{
		fnHandler	m_fnHandler;
	};

	FunctionMap	*	m_pFuncMap_LB; // Line --> Login
	FunctionMap	*	m_pFuncMap_BD; // Line --> DB
};

extern PacketHandler g_PacketHandler;

#endif
