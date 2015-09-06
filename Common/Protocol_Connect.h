#ifndef _PROTOCOLCA_CONNECT_H_INCLUDED_
#define _PROTOCOLCA_CONNECT_H_INCLUDED_

#include "Protocol.h"

/***** Client -> Login *****/
enum eCONNECT_PROTOCOL
{
	ClientType_SYN		= 100,
	ClientType_ANC			 ,
	
	ServerType_SYN 		= 200,
	ServerType_ANC			 ,
	
	Heartbeat_SYN		= 300,
	Heartbeat_ANC			 ,
};

#endif // _PROTOCOLCA_H_INCLUDED_
