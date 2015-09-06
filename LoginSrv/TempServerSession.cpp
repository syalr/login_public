#include "TempServerSession.h"
#include "LoginServer.h"

TempServerSession::TempServerSession()
	: m_bFirst(TRUE)
{
}

TempServerSession::~TempServerSession()
{
	
}

void TempServerSession::Clear()
{
	m_bFirst = TRUE;
	ServerSession::Clear();
}

void TempServerSession::OnRecv(BYTE *pMsg, WORD wSize)
{
	printf(">> TempServerSession::OnRecv\n");
	
	assert( m_bFirst == TRUE);
	if ( !m_bFirst)
		return;
	
	m_bFirst = FALSE;
	
	if ( wSize < sizeof(MSG_CONNECTION_SYN) ) {
		return;
	}
	
	MSG_CONNECTION_SYN * recvMsg = (MSG_CONNECTION_SYN *)pMsg;
		
	switch( recvMsg->ServerType )
	{
	case KEYV_SERVER:
		{
			printf("Recv KeyVServer.\n");
			//ServerSession * Obj = g_pLoginServer->GetKeyvSession();
			//assert( Obj );
			//
			//Session * pSession = this->m_pSession;
			//if ( pSession != NULL ) 
			//{
			//	m_pSession->UnbindNetworkObject();
			//	pSession->BindNetworkObject(Obj);
			//	
			//	Obj->Init();
			//}
			break;
		}
	case JSON_SERVER:
		{
			printf("Recv JsonServer.\n");
			ServerSession * Obj = g_pLoginServer->GetJsonSession();
			assert( Obj );
			
			Session * pSession = this->m_pSession;
			if ( pSession != NULL ) 
			{
				m_pSession->UnbindNetworkObject();
				pSession->BindNetworkObject(Obj);
				
				//Obj->Init();
			}
			break;
		}
	default:
		{
			// Connected warning.
			this->Release();
			return;
		}
	}
	
}

void TempServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{

}

void TempServerSession::OnLogString( char * pszLog)
{
	
}
