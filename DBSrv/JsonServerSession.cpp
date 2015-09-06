#include "JsonServerSession.h"
#include "PacketHandler.h"

JsonServerSession::JsonServerSession()
{	
}

JsonServerSession::~JsonServerSession()
{
}

void JsonServerSession::Init()
{
	
}

void JsonServerSession::OnRecv(BYTE *pMsg, WORD wSize)
{
	printf("call g_PacketHandler.ParsePacket_LD function.\n");
	g_PacketHandler.ParsePacket_LD( this, (MSG_BASE *)pMsg, wSize );
}

void JsonServerSession::OnDisconnect()
{
	ServerSession::OnDisconnect();
}

void JsonServerSession::OnLogString( char * pszLog )
{
	
}
