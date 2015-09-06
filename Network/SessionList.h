#ifndef __SESSIONLIST_H_INCLUDED_
#define __SESSIONLIST_H_INCLUDED_

#include <Utility.h>
#include <Network.h>

class SessionList 	//: public std::list<Session*> 
					: public Double_List_T<Session>
{
public:
	SessionList();
	~SessionList();
	
	void Clear();
	
	inline void Lock() { m_cs.Lock(); }
	inline void Unlock() { m_cs.Unlock(); }

private:
	Yond_mutex m_cs;
};

typedef SessionList					SESSION_LIST;
typedef SessionList::iterator		SESSION_LIST_ITER;

#endif //  __Yond_ini_INCLUDED_
