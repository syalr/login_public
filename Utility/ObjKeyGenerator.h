#ifndef _OBJKEYGENERATOR_H_INCLUDED_
#define _OBJKEYGENERATOR_H_INCLUDED_

#include <Utility.h>
#include "Yond_mutex.h"
//using namespace A;
using namespace std;

class CObjKeyGenerator
{
private:
	Yond_mutex				m_cs;
	deque<DWORD>			m_dequeKey;
	DWORD 					m_dwKeyStart;
	DWORD					m_dwKeyEnd;

public:
	CObjKeyGenerator(){}
	CObjKeyGenerator(DWORD dwKeyStart, DWORD dwKeyEnd);
	~CObjKeyGenerator(void);

	VOID					Create( DWORD dwKeyStart, DWORD dwKeyEnd );
	DWORD					GetKey();
	BOOL					ReserveKey(DWORD key);
	VOID					RestoreKey(DWORD key);
	BOOL					IsEmpty();
	BOOL					IsExistKey(DWORD dwKey);
	size_t					GetSize();
};

#endif // _OBJKEYGENERATOR_H_INCLUDED_
