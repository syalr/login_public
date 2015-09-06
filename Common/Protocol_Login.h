#ifndef _PROTOCOLCA_LOGIN_H_INCLUDED_
#define _PROTOCOLCA_LOGIN_H_INCLUDED_

#include "Protocol.h"

enum eLOGIN_PROTOCOL 
{
	// 预登陆
	PreLogin_REQ			= 200,
	PreLogin_ANC,
	PreLogin_NAK,
};

#endif // _PROTOCOLCA_H_INCLUDED_
