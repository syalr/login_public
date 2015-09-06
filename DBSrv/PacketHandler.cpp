#include "PacketHandler.h"

#include "DBServer.h"
#include "Handler_FromJsonServer.h"

PacketHandler g_PacketHandler;

PacketHandler::PacketHandler(void)
{
	m_pFuncMap_LD = new FunctionMap;
	RegisterHandler();
}

PacketHandler::~PacketHandler(void)
{
	SAFE_DELETE(m_pFuncMap_LD);
}

BOOL PacketHandler::RegisterHandler()
{
	Register_LD();
	return TRUE;
}

void PacketHandler::Register_LD()
{
	AddHandler_LD(Login_Protocol, PreLogin_REQ, Handler_FromJsonServer::OnPreLogin_REQ);
}

BOOL PacketHandler::AddHandler_LD( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_LD * pFuncInfo			= new FUNC_LD;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_LD->Add( pFuncInfo );
}

VOID PacketHandler::ParsePacket_LD( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
	assert(NULL != pMsg);
	FUNC_LD * pFuncInfo = (FUNC_LD *)m_pFuncMap_LD->Find( MAKELONG( pMsg->m_byCategory, pMsg->m_byProtocol ) );
	pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
}

