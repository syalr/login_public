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

	void Register_LD();
	BOOL AddHandler_LD( WORD category, WORD protocol, fnHandler fnHandler);
	void ParsePacket_LD( ServerSession * pUserSession, MSG_BASE * pMsg, WORD wSize );

private:
	struct FUNC_LD : public BASE_FUNC
	{
		fnHandler	m_fnHandler;
	};

	FunctionMap	*	m_pFuncMap_LD;
};

extern PacketHandler g_PacketHandler;

#endif
