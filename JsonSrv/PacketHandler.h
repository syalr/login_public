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
	void Register_LJ();
	void Register_JD();
	
	BOOL AddHandler_LJ( WORD category, WORD protocol, fnHandler fnHandler);
	BOOL AddHandler_JD( WORD category, WORD protocol, fnHandler fnHandler);

	void ParsePacket_LJ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
	void ParsePacket_JD( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

private:
	struct FUNC_LJ : public BASE_FUNC
	{
		fnHandler	m_fnHandler;
	};

	struct FUNC_JD : public BASE_FUNC
	{
		fnHandler	m_fnHandler;
	};

	FunctionMap	*	m_pFuncMap_LJ; // Line --> Login
	FunctionMap	*	m_pFuncMap_JD; // Line --> DB
};

extern PacketHandler g_PacketHandler;

#endif
