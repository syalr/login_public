#include "Yond_thread.h"


void * Yond_thread::m_fnThread( void * pt )
{
	Yond_thread * pThis = (Yond_thread *)pt;

	DWORD rt = pThis->run();

	pthread_exit(0);
}

Yond_thread::Yond_thread()
:	m_hThread ( 0 ) 
,	m_bSuspend ( 0 )
{
}

Yond_thread::~Yond_thread()
{
}

void Yond_thread::Start( BOOL bSuspend )
{
	assert( 0 == m_hThread );
	if( 0 == m_hThread )
	{
		m_bSuspend = bSuspend;
		pthread_create( &m_hThread, NULL, m_fnThread, (void*)this);
	}
}

void Yond_thread::SuspendThread()
{
	m_bSuspend = true;         
}

void Yond_thread::ResumeThread()
{
	m_bSuspend = false;         
}

