#ifndef _HANDLER_FROMJSONSERVER_H_INCLUDED_
#define _HANDLER_FROMJSONSERVER_H_INCLUDED_

#include <Common.h>

#include "ServerSession.h"

#define HANDLER_DECL(p)	static VOID On##p( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
#define HANDLER_IMPL(p)	VOID Handler_FromJsonServer::On##p( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )

class Handler_FromJsonServer
{
public:
	Handler_FromJsonServer(void);
	~Handler_FromJsonServer(void);

	HANDLER_DECL( PreLogin_REQ );
};


#endif // _HANDLER_FROMJSONSERVER_H_INCLUDED_
