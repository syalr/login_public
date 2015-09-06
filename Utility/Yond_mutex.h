#ifndef __YOND_MUTEX_INCLUDED__
#define __YOND_MUTEX_INCLUDED__

#include <Utility.h>

class Yond_mutex
{
	friend class Yond_cond;
public:
	Yond_mutex();

	~Yond_mutex();

	inline void Lock()
	{
		pthread_mutex_lock(&mutex_);
	}

	inline void Unlock()
	{
		pthread_mutex_unlock(&mutex_);
	}

	inline bool TryLock()
	{
		return (pthread_mutex_trylock(&mutex_) == 0);
	}

protected:
	pthread_mutex_t mutex_;
};

class Yond_cond
{
public:
	inline Yond_cond()
	{
		pthread_cond_init(&cond_,NULL);
	}
	inline ~Yond_cond()
	{
		pthread_cond_destroy(&cond_);
	}
	
	inline void Signal() 
	{
		pthread_cond_signal(&cond_);
	}
	inline void Broadcast() 
	{
		pthread_cond_broadcast(&cond_);
	}
	inline void Wait(Yond_mutex * lock)
	{
		pthread_cond_wait(&cond_, &lock->mutex_);
	}
	inline bool Wait(Yond_mutex * lock,int seconds)
	{
		timespec tv;
		tv.tv_nsec = 0;
		tv.tv_sec  = seconds;
		if ( pthread_cond_timedwait(&cond_, &lock->mutex_, &tv) == 0)
			return true;
		else
			return false;
	}
	
private:
	pthread_cond_t cond_;
};

class Yond_guard
{
private :
	Yond_mutex & mtx_;
	
public:
	Yond_guard(Yond_mutex & mtx) 
		: mtx_(mtx) 
	{
		mtx_.Lock();
	} 
	~Yond_guard() {
		mtx_.Unlock();
	}
};

#endif
