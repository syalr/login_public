#ifndef __Yond_Thread_INCLUDED_
#define __Yond_Thread_INCLUDED_

#include "Utility.h"

typedef int (*fcallback_running)();

class Yond_thread
{ 
public: 
    Yond_thread(); 
    virtual ~Yond_thread(); 
 
    void Start(BOOL bSuspend = FALSE);
	virtual void EndThread() = 0;

	virtual void SuspendThread();
	virtual void ResumeThread();
	
	pthread_t GetHandle() { return m_hThread; }
	
protected:
	virtual DWORD	run() = 0;
	static void * m_fnThread(void * pt);
	
private: 
	pthread_t	m_hThread;
	BOOL		m_bSuspend;
};

#endif //  __Yond_ini_INCLUDED_
