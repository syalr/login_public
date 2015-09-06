#include "SessionList.h"
#include "Session.h"

SessionList::SessionList() 
{
	
}

SessionList::~SessionList()
{
	Clear();
}

void SessionList::Clear()
{
	SESSION_LIST_ITER it;

	Session *pSession; 

	this->Lock();

	while ( !this->empty() )
	{
		pSession = this->pop_front();
		
		if ( pSession )
			delete pSession;		
	}

	Unlock();
}
 