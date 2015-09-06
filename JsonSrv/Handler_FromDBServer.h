#ifndef _HANDLER_FROMADBSERVER_H_INCLUDED_
#define _HANDLER_FROMADBSERVER_H_INCLUDED_

#include <Common.h>
#include "ServerSession.h"

#define HANDLER_DECL( p )			static VOID On##p( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
#define HANDLER_IMPL( p )			VOID Handler_FromDBServer::On##p( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )

class Handler_FromDBServer
{
public:
	Handler_FromDBServer(void);
	~Handler_FromDBServer(void);

	HANDLER_DECL( PreLogin_ANC );
	HANDLER_DECL( PreLogin_NAK );
};


#endif