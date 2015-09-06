#ifndef _RECVBUFFER_H_
#define _RECVBUFFER_H_

#include <Utility.h>
#include <Network.h>

//=============================================================================================================================
/// 
//		recv  GQCS completion Completion(int)
//=============================================================================================================================
class RecvBuffer
{
public:
	RecvBuffer()			{ m_pQueue = NULL; }
	virtual ~RecvBuffer()	{ if( m_pQueue ) delete m_pQueue; }

	inline void	Create( int nBufferSize, DWORD dwExtraBufferSize ) {
		if( m_pQueue ) delete m_pQueue;
		m_pQueue = new CircuitQueue<BYTE>;
		m_pQueue->Create( nBufferSize, dwExtraBufferSize ); }

	inline void	Completion( int nBytesRecvd ) { m_pQueue->Enqueue( NULL, nBytesRecvd ); }

	inline void	Clear() { m_pQueue->Clear(); }

	inline void	GetRecvParam( BYTE **ppRecvPtr, int &nLength ) {
		*ppRecvPtr	= m_pQueue->GetWritePtr();		
		nLength		= m_pQueue->GetWritableLen(); }

	inline BYTE* GetFirstPacketPtr() {
		PACKET_HEADER   header;
		int nPktSize = sizeof(PACKET_HEADER);

		if( !m_pQueue->Peek( (BYTE*)&header, nPktSize ) )
			return NULL;

		nPktSize += header.size;
		if( m_pQueue->GetLength() < nPktSize )	
			return NULL;
		
		if( m_pQueue->GetBackDataCount() < nPktSize ) 
		{
			m_pQueue->CopyHeadDataToExtraBuffer( nPktSize - m_pQueue->GetBackDataCount() ); 
		}
		return  m_pQueue->GetReadPtr();	}

	inline void	RemoveFirstPacket( WORD wSize ) { m_pQueue->Dequeue( NULL, wSize ); }

private:
	CircuitQueue<BYTE>	*m_pQueue;
};

#endif
