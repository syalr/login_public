#include "ObjKeyGenerator.h"
//#include <vector>
//#include <algorithm>

CObjKeyGenerator::CObjKeyGenerator( DWORD dwKeyStart, DWORD dwKeyEnd )
{
	Create(dwKeyStart, dwKeyEnd);

}
VOID CObjKeyGenerator::Create( DWORD dwKeyStart, DWORD dwKeyEnd )
{
	for( DWORD i = dwKeyStart; i <= dwKeyEnd; ++i )
	{
		m_dequeKey.push_back( i );
	}
}

CObjKeyGenerator::~CObjKeyGenerator(void)
{
	m_dequeKey.clear();
}

DWORD CObjKeyGenerator::GetKey()
{
	DWORD key;

	if ( IsEmpty() == TRUE ) { 
		return NULL;
	}

	m_cs.Lock();

	key = *m_dequeKey.begin();
	m_dequeKey.pop_front();

	m_cs.Unlock();

	return key;
}

BOOL CObjKeyGenerator::ReserveKey( DWORD key )
{
	BOOL bReserved = FALSE;

	if (IsEmpty() == TRUE) { 
		return FALSE;
	}

	m_cs.Lock();

	deque<DWORD>::iterator itr = m_dequeKey.begin();
	while ( itr != m_dequeKey.end() )
	{
		if ( *itr == key )
		{
			bReserved = TRUE;
			m_dequeKey.erase(itr);
			printf("m_dequeKey.erase(%d) Success\n", key);
			break;
		}
	}

	return bReserved;
}


BOOL CObjKeyGenerator::IsEmpty()
{
	return m_dequeKey.empty() == TRUE; 
}	

BOOL CObjKeyGenerator::IsExistKey(DWORD dwKey)
{
	deque<DWORD>::iterator itr = m_dequeKey.begin();

	for (; itr != m_dequeKey.end(); itr++)
	{
		if (*itr == dwKey)
		{
			return TRUE;
		}
	}

	return FALSE;
}

VOID CObjKeyGenerator::RestoreKey(DWORD key)
{		 
	Yond_guard sync_cs(m_cs);
	m_dequeKey.push_back(key);
	printf("[CObjKeyGenerator::RestoreKey] m_dequeKey.push_back(%d)\n", key);
}

size_t CObjKeyGenerator::GetSize()
{
	Yond_guard sync_cs(m_cs);
	return m_dequeKey.size();
}



