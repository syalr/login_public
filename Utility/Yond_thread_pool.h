#ifndef __YONDTHREADPOOL_H_INCLUDED_
#define __YONDTHREADPOOL_H_INCLUDED_

//=======================================================================================================================
/// IThreadInterface 
/**
	@author
	@since
	@remarks
	@history
*/
//=======================================================================================================================

#include <vector>
using namespace std;


class Yond_thread;

class Yond_thread_pool
{
public:
	Yond_thread_pool();
	~Yond_thread_pool();

	void								AddThread( 
										Yond_thread * pThread, 
										BOOL bSuspend = FALSE );
										
	inline void							SetAutoRelease( BOOL bVal ) { m_bAutoRelease = bVal; }
	void								BatchStopThread();
	void								BatchStartThread();
	void								Release();

private:
	vector<Yond_thread *>				m_pThreadList;
	BOOL								m_bAutoRelease;
};




#endif // __THREADPOOL_H__
	
	

