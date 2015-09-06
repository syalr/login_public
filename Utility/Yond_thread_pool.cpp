#include <Utility.h>
#include "Yond_thread_pool.h"
#include "Yond_thread.h"


Yond_thread_pool::Yond_thread_pool()
	: m_bAutoRelease	( TRUE )
{
}

Yond_thread_pool::~Yond_thread_pool()
{
	Release();
}
	

void Yond_thread_pool::Release()
{
	BatchStopThread();

	Yond_thread * pThread = NULL;
	for ( int i = 0; i < m_pThreadList.size(); i ++ )
	{
		pThread = m_pThreadList[i];
		if( m_bAutoRelease )
			delete pThread;
	}

	m_pThreadList.clear();
}

void Yond_thread_pool::AddThread( Yond_thread * pThread, BOOL bSuspend )
{
	m_pThreadList.push_back(pThread);
	pThread->Start( bSuspend );
}

void Yond_thread_pool::BatchStopThread()
{
	Yond_thread * pThread = NULL;
	for ( int i = 0; i < m_pThreadList.size(); i ++ )
	{
		pThread = m_pThreadList[i];
		pThread->EndThread();

#if defined(WIN32) || defined( WIN64 )
		WaitForSingleObject(pThread->GetHandle(), INFINITE);
#else
		pthread_join(pThread->GetHandle(), NULL);
#endif

	}
}

void Yond_thread_pool::BatchStartThread()
{
	Yond_thread * pThread = NULL;
	for ( int i = 0; i < m_pThreadList.size(); i ++ )
	{
		pThread = m_pThreadList[i];
		pThread->ResumeThread();
	}
}

 

