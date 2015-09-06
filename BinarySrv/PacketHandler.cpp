#include "PacketHandler.h"

#include "Handler_FromDBServer.h"
#include "Handler_FromLoginServer.h"

PacketHandler g_PacketHandler;

PacketHandler::PacketHandler(void)
{
	m_pFuncMap_LB = new FunctionMap;
	m_pFuncMap_BD = new FunctionMap;
	RegisterHandler();
}

PacketHandler::~PacketHandler(void)
{
	SAFE_DELETE(m_pFuncMap_LB);
	SAFE_DELETE(m_pFuncMap_BD);
}

BOOL PacketHandler::RegisterHandler()
{
	Register_LB();
	Register_BD();
	return TRUE;
}

void PacketHandler::Register_LB()
{	
	AddHandler_LB( Login_Protocol, PreLogin_REQ, Handler_FromLoginServer::OnPreLogin_REQ );	// 预登陆
}

void PacketHandler::Register_BD()
{
	AddHandler_BD( Login_Protocol, PreLogin_ANC, Handler_FromDBServer::OnPreLogin_ANC );
	AddHandler_BD( Login_Protocol, PreLogin_NAK, Handler_FromDBServer::OnPreLogin_NAK );
}

BOOL PacketHandler::AddHandler_LB( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_LB * pFuncInfo	= new FUNC_LB;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_LK->Add( pFuncInfo );
}

BOOL PacketHandler::AddHandler_BD( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_BD * pFuncInfo	= new FUNC_BD;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_KD->Add( pFuncInfo );
}

VOID PacketHandler::ParsePacket_LB( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
	assert( NULL != pMsg );
	FUNC_LB * pFuncInfo = (FUNC_LB *)m_pFuncMap_LK->Find( MAKELONG( pMsg->m_byCategory, pMsg->m_byProtocol ) );
	if ( pFuncInfo == NULL ) {
		printf("[PacketHandler::ParsePacket_LK] Error\n");
		return;
	}
	
	pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
}

VOID PacketHandler::ParsePacket_BD( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
	assert( NULL != pMsg );
	FUNC_BD * pFuncInfo = (FUNC_BD *)m_pFuncMap_BD->Find( MAKELONG( pMsg->m_byCategory, pMsg->m_byProtocol ) );
	if ( pFuncInfo == NULL ) {
		printf("[PacketHandler::ParsePacket_KD] Error\n");
		return;
	}
	pFuncInfo->m_fnHandler( pSession, pMsg, wSize );}
