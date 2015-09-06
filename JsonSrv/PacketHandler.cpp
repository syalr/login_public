#include "PacketHandler.h"
#include "Handler_FromDBServer.h"
#include "Handler_FromLoginServer.h"

PacketHandler g_PacketHandler;

PacketHandler::PacketHandler(void)
{
	m_pFuncMap_LJ = new FunctionMap;
	m_pFuncMap_JD = new FunctionMap;
	RegisterHandler();
}

PacketHandler::~PacketHandler(void)
{
	SAFE_DELETE(m_pFuncMap_LJ);
	SAFE_DELETE(m_pFuncMap_JD);
}

BOOL PacketHandler::RegisterHandler()
{
	Register_LJ();
	Register_JD();
	return TRUE;
}

void PacketHandler::Register_LJ()
{	
	AddHandler_LJ( Login_Protocol, PreLogin_REQ, Handler_FromLoginServer::OnPreLogin_REQ );	// 预登陆
}

void PacketHandler::Register_JD()
{
	AddHandler_JD( Login_Protocol, PreLogin_ANC, Handler_FromDBServer::OnPreLogin_ANC );
	AddHandler_JD( Login_Protocol, PreLogin_NAK, Handler_FromDBServer::OnPreLogin_NAK );
}

BOOL PacketHandler::AddHandler_LJ( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_LJ * pFuncInfo	= new FUNC_LJ;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_LJ->Add( pFuncInfo );
}

BOOL PacketHandler::AddHandler_JD( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_JD * pFuncInfo	= new FUNC_JD;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_JD->Add( pFuncInfo );
}

VOID PacketHandler::ParsePacket_LJ( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
	assert( NULL != pMsg );
	FUNC_LJ * pFuncInfo = (FUNC_LJ *)m_pFuncMap_LJ->Find( MAKELONG( pMsg->m_byCategory, pMsg->m_byProtocol ) );
	if ( pFuncInfo == NULL ) {
		printf("[PacketHandler::ParsePacket_LJ] Error\n");
		return;
	}
	
	pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
}

VOID PacketHandler::ParsePacket_JD( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
	assert( NULL != pMsg );
	FUNC_JD * pFuncInfo = (FUNC_JD *)m_pFuncMap_JD->Find( MAKELONG( pMsg->m_byCategory, pMsg->m_byProtocol ) );
	if ( pFuncInfo == NULL ) {
		printf("[PacketHandler::ParsePacket_JD] Error\n");
		return;
	}
	pFuncInfo->m_fnHandler( pSession, pMsg, wSize );}
