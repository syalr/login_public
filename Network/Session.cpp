#include "Session.h"
using namespace SocketOpt;

Session::Session(DWORD dwSendBufferSize, DWORD dwRecvBufferSize, DWORD dwMaxPacketSize, DWORD dwTimeOut)
{
	m_pSendBuffer = new SendBuffer;
	m_pSendBuffer->Create(dwSendBufferSize, dwMaxPacketSize);
	m_pSendBuffer->Clear();
	
	m_pRecvBuffer = new RecvBuffer;
	m_pRecvBuffer->Create(dwRecvBufferSize, dwMaxPacketSize);
	m_pRecvBuffer->Clear();
	
	m_dwTimeOut 	= dwTimeOut;
	
	m_socket 		= INVALID_SOCKET;
	
	m_bCanSend 		= TRUE;
	m_bAcceptSocket = FALSE;
	m_bDisconnectOrdered = FALSE;
		
	m_pNetworkObject = NULL;
	
	m_dwTotalRecvBytes 	= 0;
	m_dwTotalSendBytes 	= 0;
	m_dwLastSyncTick 	= 0;
	m_bRemove = FALSE;
	m_dwIndex = 0;

	m_iCategory = 0;
	m_iProtocol = 0;
	m_iSize 	= 0;	
}

Session::~Session()
{
	CloseSocket();
	
	if ( m_pSendBuffer )	delete m_pSendBuffer;
	if ( m_pRecvBuffer )	delete m_pRecvBuffer;
}

void Session::Init()
{
	m_pSendBuffer->Clear();
	m_pRecvBuffer->Clear();
	
	ResetKillFlag();
	
	m_bDisconnectOrdered = FALSE;
	m_bCanSend			 = TRUE;
	
	m_dwTotalRecvBytes	 = 0;
	m_dwTotalSendBytes	 = 0;
}

BOOL Session::Send( BYTE * pMsg, WORD wSize )
{
	PACKET_HEADER header;
	header.size = wSize;
	if ( m_pSendBuffer->Write(&header, pMsg) == FALSE )
	{
		this->Remove();
	}
	
	m_dwTotalSendBytes	+= wSize;
	return TRUE;
}

BOOL Session::SendEx( DWORD dwNumberOfMessages, BYTE ** ppMsg, WORD *pwSize)
{
	PACKET_HEADER header;
	header.size = 0;
	DWORD i;
	for(i=0; i<dwNumberOfMessages; ++i) {
		header.size	+=	pwSize[i];
	}
	if ( !m_pSendBuffer->Write( (BYTE*)&header, sizeof(PACKET_HEADER)) )
	{
		this->Remove();
		return FALSE;
	}
	for (i = 0; i < dwNumberOfMessages; ++i )
	{
		if ( !m_pSendBuffer->Write( ppMsg[i], pwSize[i] ) )
		{
			this->Remove();
			return FALSE;
		}
	}
}

BOOL Session::ProcessRecvdPacket( DWORD dwMaxPacketSize)
{
	BYTE 			* pPacket;
	PACKET_HEADER	* pHeader;
	
	//
	while ( pPacket = GetRecvBuffer()->GetFirstPacketPtr() )
	{
		pHeader = (PACKET_HEADER*) pPacket;
		if ( pHeader->size <= 0 ) {
			return FALSE;
		}
		if ( pHeader->size + sizeof(PACKET_HEADER) > dwMaxPacketSize) {
			return FALSE;
		}
		m_pNetworkObject->OnRecv( pPacket + sizeof(PACKET_HEADER), pHeader->size );
		GetRecvBuffer()->RemoveFirstPacket( sizeof(PACKET_HEADER) + pHeader->size );
		
		ResetTimeOut();
		m_iCategory = *( pPacket + sizeof(PACKET_HEADER) );
		m_iProtocol = *( pPacket + sizeof(PACKET_HEADER) + 1 );
		m_iSize	= pHeader->size + sizeof(PACKET_HEADER);
		
		m_dwTotalRecvBytes	+= pHeader->size + sizeof(PACKET_HEADER);
	}
	return TRUE;
}

BOOL Session::OnSend()
{
	m_lockSend.Lock();
	m_bCanSend = TRUE;
	m_lockSend.Unlock();
	return m_bCanSend;
}

BOOL Session::PreSend(SyncHandler * pSync)
{
	if ( !m_bRemove && m_bCanSend && m_pSendBuffer->IsReadyToSend() ) {
		struct epoll_event event;
		event.events = 0x800;
		event.data.ptr = this;
		pSync->AddIoEvent(&event);
		//printf("pSync->AddIoEvent():: Ready to send.\n");
	}
	return TRUE;
}

BOOL Session::DoSend(SyncHandler * pSync)
{
	Yond_guard gd(m_lockSend);
	if (m_bCanSend && m_bRemove ==FALSE) 
	{
		BYTE * buf;
		int len, lenEx = 0;
		WORD hander = 0;
		
		lenEx = 0;
		if ( m_pNetworkObject != NULL ) {
			if ( m_pNetworkObject->IsPackageHeader() == FALSE ) {
				lenEx = sizeof(WORD);
			}
		}
		
		if (m_pSendBuffer->GetSendParam( &buf, len ) == FALSE)
			return TRUE;
		
		int ret = send( m_socket, (buf + lenEx), ( len - lenEx ), 0);
		if (ret == SOCKET_ERROR)
		{
			if (errno == EAGAIN)
			{
				ret = 0;
			}
			else
			{
				return FALSE;
			}
			if (ret < len)
			{
				m_bCanSend = FALSE;
				DWORD event = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLERR | EPOLLHUP;
				pSync->ModEpollEvent(this, event);
			}
		}
		
		m_pSendBuffer->Completion( (ret + lenEx) );
	}
}

BOOL Session::DoRecv()
{
	Yond_guard gd(m_lockSend);
	
	BYTE * buf;
	int ret = 0, len = 0, lenEx = 0;
	WORD hander = 0; 
	
	while(m_bRemove == FALSE) 
	{
		m_pRecvBuffer->GetRecvParam( &buf, len );
		if ( len <= 0 ) {
			this->Remove();
			return FALSE;
		}
		
		lenEx = 0;
		if ( m_pNetworkObject != NULL ) {
			if ( m_pNetworkObject->IsPackageHeader() == FALSE ) {
				lenEx = sizeof(WORD);
			}
		}
		
		ret = recv(m_socket, (buf + lenEx), ( len - lenEx ), 0);
		if (ret == SOCKET_ERROR)
		{
			if (errno == EAGAIN) {
				return TRUE;
			}
			else {
				this->Remove();
				return FALSE;
			}
		}
		if (ret == 0) {
			this->Remove();
			return FALSE;
		}
		
		hander = ret;
		memcpy( buf, &hander, lenEx );
		m_pRecvBuffer->Completion( ( ret + lenEx ) );
		
		if (ret < len)
			break;
	}
	return TRUE;
}

BOOL Session::CreateSocket()
{
	int nRet		= 0;
	int nZero		= 0;

	SOCKET newSocket = socket(AF_INET, SOCK_STREAM, 0);

	if( newSocket == INVALID_SOCKET ) 
	{
		return newSocket;
	}

	SocketOpt::Nonblocking( newSocket );
	SocketOpt::DisableBuffering( newSocket );
	return newSocket;
}

void Session::BindNetworkObject( NetworkObject *pNetworkObject )
{
	m_pNetworkObject = pNetworkObject;
	pNetworkObject->SetSession( this );
}

void Session::UnbindNetworkObject()
{
	if( m_pNetworkObject == NULL ) {
		return;
	}

	m_pNetworkObject->SetSession( NULL );
	m_pNetworkObject = NULL;
}

void Session::OnAccept()
{
	ResetKillFlag();
	ResetTimeOut();
	m_pNetworkObject->OnAccept( GetIndex() );
}

void Session::OnConnect( BOOL bSuccess )
{
	this->Init();
	
	NetworkObject *pNetworkObject = m_pNetworkObject;

	if( !bSuccess ) {
		UnbindNetworkObject();
	}

	pNetworkObject->OnConnect( bSuccess, GetIndex() );
}

void Session::Disconnect( BOOL bGracefulDisconnect )
{
	printf( "[Session::Disconnect = %d \n]", bGracefulDisconnect ) ;
	if( bGracefulDisconnect ) { 
		this->Remove(); 
	} 
	else {
		m_bDisconnectOrdered = TRUE; 
	}
}

void Session::CloseSocket()
{
	SocketOpt::CloseSocket(m_socket);
	m_socket = INVALID_SOCKET;
}

DWORD Session::GetTickCount()
{
	static long long t0 = 0;
	struct timeval tv;
	
	gettimeofday(&tv, NULL);
	long long t = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	
	if (t0 == 0)
		t0 = t;
	return t - t0;
}

void Session::OnLogString(char * pszLog, ...)
{
	if ( !m_pNetworkObject )
		return;
	
	char szBuffer[512] = " ";
	va_list pArguments;
	
	va_start( pArguments, pszLog );
	vsprintf( szBuffer, pszLog, pArguments );
	va_end( pArguments );
	
	printf("%s(%s, %d) \n", szBuffer, GetIP().c_str(), GetIndex());
	m_pNetworkObject->OnLogString(szBuffer);
}
