#ifndef _NETWORK_H_INCLUDED_
#define _NETWORK_H_INCLUDED_

class Connector;
class Acceptor;
class Session;
class SessionList;
class SessionPool;
class NetworkObject;
class SyncHandler;
class IOCPServer;

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>	
#include <assert.h>
#include <errno.h>	
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <unistd.h>	
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <string.h>	

typedef int64_t		INT64;
typedef int32_t		INT32;
typedef int16_t		INT16;
typedef int8_t		INT8;
typedef uint64_t	UINT64;
typedef uint32_t	UINT32;
typedef uint16_t	UINT16;
typedef uint8_t		UINT8;
typedef int			SOCKET;
typedef int         BOOL;

typedef uint8_t     BYTE;
typedef uint16_t    WORD;
typedef uint32_t	DWORD;
typedef uint16_t	USHORT;

typedef float               FLOAT;
typedef double				DOUBLE;
typedef int                 INT;
typedef unsigned int        UINT;
typedef char				CHAR;
typedef short				SHORT;
typedef unsigned short		USHORT;
typedef char				*LPSTR, *PSTR;
typedef const char			*LPCSTR, *PCSTR;

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

typedef std::map<DWORD,  SyncHandler*>	SYNCHANDLER_MAP;
typedef std::pair<DWORD, SyncHandler*>	SYNCHANDLER_MAP_PAIR;
typedef SYNCHANDLER_MAP::iterator		SYNCHANDLER_MAP_ITER;

typedef NetworkObject* (*fnCallBackCreateAcceptedObject)();
typedef void (*fnCallBackDestroyAcceptedObject)( NetworkObject *pNetworkObject );
typedef void (*fnCallBackDestroyConnectedObject)( NetworkObject *pNetworkObject );

//-------------------------------------------------------------------------------------------------
/// I/O 
//-------------------------------------------------------------------------------------------------
typedef struct tagSYNCHANDLER_DESC
{
	DWORD								dwSyncHandlerKey;				///< I/O 
	DWORD								dwMaxAcceptSession;				///< 
	DWORD								dwMaxConnectSession;			///< 
	DWORD								dwSendBufferSize;				///< 
	DWORD								dwRecvBufferSize;				///< 
	DWORD								dwTimeOut;						///< 
	DWORD								dwMaxPacketSize;				///< 
	DWORD								dwNumberOfIoThreads;			///< I/O 
	DWORD								dwNumberOfAcceptThreads;		///< Accept 
	DWORD								dwNumberOfConnectThreads;		///< Connect 
	fnCallBackCreateAcceptedObject		fnCreateAcceptedObject;			///< NetworkObject 
	fnCallBackDestroyAcceptedObject		fnDestroyAcceptedObject;		///< NetworkObject 
	fnCallBackDestroyConnectedObject	fnDestroyConnectedObject;		///< NetworkObject 
} SYNCHANDLER_DESC, *LPSYNCHANDLER_DESC;
typedef tagSYNCHANDLER_DESC IOHANDLER_DESC, *LPIOHANDLER_DESC;

#pragma pack(push,1)
typedef struct tagPACKET_HEADER {
	WORD	size;		/// 
} PACKET_HEADER;
#pragma pack(pop)

#define TRUE	1
#define FALSE	0
	
#define INVALID_SOCKET	-1
#define	SOCKET_ERROR	-1
	
#define SAFE_DELETE( p)				{ if ( p) { delete ( p); ( p) = NULL; } }
#define Sleep(ms) usleep(1000*ms)

#define SOCKET_HOLDER_SIZE 1024

#define MAX_IO_WORKER_THREAD 16
#define EXTRA_ACCEPTEX_NUM	 10

#include "SocketOpt.h"
#include "CircuitQueue.h"
#include "SendBuffer.h"
#include "RecvBuffer.h"
#include "Yond_thread.h"
#include "Yond_mutex.h"
#include "Double_List_T.h"
#include "Connector.h"
#include "Acceptor.h"
#include "NetworkObject.h"
#include "Session.h"
#include "SessionPool.h"
#include "SessionList.h"
#include "SyncHandler.h"
#include "IOCPServer.h"

#endif