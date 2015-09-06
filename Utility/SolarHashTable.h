#ifndef _SOLARHASHTABLE_H_INCLUDED_
#define _SOLARHASHTABLE_H_INCLUDED_

#include <Utility.h>

using namespace A;

namespace A
{

template <class T, class KeyType=DWORD >
class SolarHashTable
{	
	class HashBucket;

	class HashList
	{
	public:
		HashList *		pNext;
		HashList *		pPrv;
		HashBucket *	pBucket;
	};

	class HashBucket
	{
	public:
		KeyType			dwKey;
		T				TData;
		HashList *		pList;		// 
		HashBucket *	pNext;		// 
		HashBucket *	pPrv;
		void SetData(T pvoid, KeyType key) { TData = pvoid;dwKey = key; pNext = NULL; pPrv = NULL; }
	};

	BOOL				m_bInited;
protected:
	DWORD				m_dwDataNum;
	DWORD				m_dwMaxBucketNum;
	HashBucket **		m_ppBucketTable;

	// for iterator
	HashList *			m_pListTail;
	HashList * 			m_pListHead;
	HashList *			m_pListCur;
	
	// for bucket iterator
	HashBucket *		m_pCurBucket;
	HashBucket*			m_pLastBucket;
	KeyType				m_dwBucketKey;

	// memorypool
	MemoryFactory<HashBucket>	* m_pHashBucketPool;
	MemoryFactory<HashList>		* m_pHashListPool;

public:

	SolarHashTable()
	{
		m_dwDataNum			= 0;
		m_dwMaxBucketNum	= 0;
		m_ppBucketTable		= NULL;
		m_pListHead			= NULL;
		m_pListTail			= NULL;
		m_pListCur			= NULL;
		m_pCurBucket		= NULL;
		m_pHashListPool		= NULL;
		m_pHashBucketPool	= NULL;
		m_bInited			= FALSE;
	}

	SolarHashTable( DWORD dwMaxBucketNum )
	{
		m_dwDataNum			= 0;
		m_dwMaxBucketNum	= 0;
		m_ppBucketTable		= NULL;
		m_pListHead			= NULL;
		m_pListTail			= NULL;
		m_pListCur			= NULL;
		m_pCurBucket		= NULL;
		m_pHashListPool		= NULL;
		m_pHashBucketPool	= NULL;
		m_bInited			= FALSE;

		Initialize( dwMaxBucketNum );
	}
	
	virtual ~SolarHashTable()
	{
		RemoveAll();
		if (m_ppBucketTable)
		{
			delete [] m_ppBucketTable;
			m_ppBucketTable = NULL;
		}
		
		if(m_pHashListPool)
		{
			m_pHashListPool->Release();
			delete m_pHashListPool;
			m_pHashListPool = NULL;
		}
		if( m_pHashBucketPool )
		{
			m_pHashBucketPool->Release();
			delete m_pHashBucketPool;
			m_pHashBucketPool = NULL;
		}
	}
	
	inline KeyType		GetDataNum() { return m_dwDataNum; }
	BOOL				Initialize(DWORD dwMaxBucketNum)
	{
		if( m_bInited ) return FALSE;

		assert( NULL == m_pHashBucketPool );
		assert( NULL == m_pHashListPool );
		m_dwDataNum			= 0;
		m_dwMaxBucketNum	= dwMaxBucketNum;
		m_ppBucketTable		= NULL;
		m_pListHead			= NULL;
		m_pListTail			= NULL;
		m_pListCur			= NULL;
		m_pCurBucket		= NULL;
		
		m_pHashBucketPool	= new MemoryFactory<HashBucket>;
		m_pHashListPool		= new MemoryFactory<HashList>;
		m_pHashBucketPool->Initialize( dwMaxBucketNum, dwMaxBucketNum/2+1 );
		m_pHashListPool->Initialize( dwMaxBucketNum, dwMaxBucketNum/2+1 );
		
		m_ppBucketTable = new HashBucket *[m_dwMaxBucketNum];
		
		if( !m_ppBucketTable )
			return FALSE;
		
		memset(m_ppBucketTable, 0, sizeof(HashBucket *) * m_dwMaxBucketNum);
		m_bInited			= TRUE;
		return TRUE;
		
	}
	// don't check for the same key
	BOOL				Add(T TData, KeyType dwKey)
	{
		KeyType index = dwKey % m_dwMaxBucketNum;
		
		HashBucket *	cur = NULL;
		HashBucket *	prv = NULL;
		//HashBucket *	next = NULL;
		
		
		if (!m_ppBucketTable[index])
		{
			
			m_ppBucketTable[index] = m_pHashBucketPool->Alloc();
			m_ppBucketTable[index]->SetData( TData, dwKey );
			m_ppBucketTable[index]->pList = addList(m_ppBucketTable[index]);

		}
		else 
		{
			cur = m_ppBucketTable[index];
			while (cur)
			{
				prv = cur;
				cur = cur->pNext;
			}
			cur = prv->pNext = m_pHashBucketPool->Alloc();
			cur->SetData( TData, dwKey );
			cur->pPrv = prv;
			cur->pList = addList(cur);
			
		}
		m_dwDataNum++;
		
		return TRUE;
	}
	
	T					GetData(KeyType dwKey)
	{
		KeyType index = dwKey % m_dwMaxBucketNum;
		
		HashBucket* pBucket = m_ppBucketTable[index];
		
		while(pBucket)
		{
			if (pBucket->dwKey == dwKey)
			{
				return pBucket->TData;
			}
			pBucket = pBucket->pNext;
		}
		return NULL;
		
	}
	
	T					GetHeadData() { if( !m_pListHead ) return NULL; return m_pListHead->pBucket->TData;	}
	T					GetTailData() { if( !m_pListTail ) return NULL; return m_pListTail->pBucket->TData;	}
	
	// Iterate List
	inline void			SetFirst() { m_pListCur = m_pListHead; }
	T			 		GetNext()
	{
		if (m_pListCur)
		{
			const T& TData = m_pListCur->pBucket->TData;
			m_pListCur = m_pListCur->pNext;
			return TData;
		}

		return NULL;		
	}
	bool			 		GetNextDataAndKey(T &TData, KeyType &dwKey)
	{
		if (m_pListCur)
		{
			TData = m_pListCur->pBucket->TData;
			dwKey = m_pListCur->pBucket->dwKey;
			m_pListCur = m_pListCur->pNext;
			return true;
		}

		return false;		
	}
 
	inline void			SetLast() { m_pListCur = m_pListTail;	}
	T			 		GetPrev()
	{
		if (m_pListCur)
		{
			const T& TData = m_pListCur->pBucket->TData;
			m_pListCur = m_pListCur->pPrv;
			return TData;
		}

		return NULL;		
	}


	
	// Iterate Bucket Chain List
	void				SetBucketFirst(KeyType dwKey)
	{
		KeyType index = dwKey % m_dwMaxBucketNum;			
		m_dwBucketKey = dwKey;
		m_pCurBucket = m_ppBucketTable[index];
		m_pLastBucket = NULL;
	}		
	T					GetBucketNext()
	{
		while(m_pCurBucket)
		{
			if (m_pCurBucket->dwKey == m_dwBucketKey)
			{
				T	TData = m_pCurBucket->TData;
				m_pLastBucket = m_pCurBucket;
				m_pCurBucket = m_pCurBucket->pNext;
				return TData;
			}
			m_pCurBucket = m_pCurBucket->pNext;
		}
		return NULL;
	}
	void				RemoveCurBucketData()
	{
		KeyType dwIndex = m_dwBucketKey%m_dwMaxBucketNum;
		HashBucket*	cur = m_pLastBucket;
		HashBucket*	prv = NULL;
		HashBucket*	next = NULL;

		prv = cur->pPrv;
		next = cur->pNext;
		if (!prv)
			m_ppBucketTable[dwIndex] = next;
		else 
			prv->pNext = next;

		if (next)
			next->pPrv = prv;

		removeList(cur->pList);
		m_pHashBucketPool->Free(cur);
		cur = NULL;
		--m_dwDataNum;
	}

	
	// delete bucket, list for (dwKey)
	void				Remove(KeyType dwKey)
	{
		KeyType dwIndex = dwKey % m_dwMaxBucketNum;
		
		HashBucket*	cur = m_ppBucketTable[dwIndex];
		HashBucket*	prv = NULL;
		HashBucket*	next = NULL;
		
		while (cur)
		{
			if (cur->dwKey == dwKey)
			{
				prv = cur->pPrv;
				next = cur->pNext;

				if(cur == m_pCurBucket)			/// for iterator sync
					m_pCurBucket = next;

				if (!prv)
					m_ppBucketTable[dwIndex] = next;
				else 
					prv->pNext = next;
				
				if (next)
					next->pPrv = prv;
				
				removeList(cur->pList);
				m_pHashBucketPool->Free(cur);//delete cur;
				cur = NULL;
				--m_dwDataNum;
				return;
			}
			cur = cur->pNext;
		}
	}

	T					RemoveHead()
	{
		HashList*	cur = m_pListHead;
		if( !cur ) return NULL;
		m_pListHead = m_pListHead->pNext;
		T data = cur->pBucket->TData;

		m_pHashBucketPool->Free(cur->pBucket);	//delete cur->pBucket;
		m_pHashListPool->Free(cur);				//delete cur;

		--m_dwDataNum;

		return data;
	}
	// delete all of Bucket, List Node
	void				RemoveAll()
	{
		HashList*	cur = m_pListHead;
		HashList*	next = NULL;
		
		while (cur)
		{
			next = cur->pNext;
			m_pHashBucketPool->Free(cur->pBucket);	//delete cur->pBucket;
			m_pHashListPool->Free(cur);				//delete cur;
			cur = next;
		}
		m_dwDataNum = 0;
		m_pListHead = NULL;
		
		if(m_ppBucketTable)
			memset(m_ppBucketTable, 0, sizeof(HashBucket *) * m_dwMaxBucketNum);
	}
	
	//------------------------------------------------------------------------------
	// iterator 
	//------------------------------------------------------------------------------
	class iterator
	{
		friend class SolarHashTable;
		HashList * _current;

	public:
		iterator():_current(NULL){}
		iterator( const iterator & itr ):_current(itr._current){}
		~iterator(){}

		iterator & operator++()
		{
			if( _current ) _current = _current->pNext;
			return *this;
		}
		iterator & operator++( int i )
		{
			if( _current ) _current = _current->pNext;
			return *this;
		}
		iterator & operator--()
		{
			if( _current ) _current = _current->pPrv;
			return *this;
		}
		iterator & operator--( int i )
		{
			if( _current ) _current = _current->pPrv;
			return *this;
		}

		BOOL operator!=( iterator &it )
		{
			return ( it._current != _current );
		}
		
		BOOL operator!=( iterator it )
		{
			return ( it._current != _current );
		}

		BOOL operator==( iterator & it )
		{
			return ( it._current == _current );
		}

		T operator*()
		{
			if( _current ) return _current->pBucket->TData;
			return NULL;
		}
	};

	iterator find( KeyType dwKey )
	{
		KeyType index = dwKey % m_dwMaxBucketNum;

		HashBucket* pBucket = m_ppBucketTable[index];

		while(pBucket)
		{
			if (pBucket->dwKey == dwKey)
			{
				break;
			}
			pBucket = pBucket->pNext;
		}
		iterator it;
		if( pBucket )
			it._current = pBucket->pList;
		else
			it._current = NULL;
		return it;
	}

	iterator begin()
	{
		iterator it;
		it._current = m_pListHead;
		return it;
	}
	iterator end()
	{
		iterator it;
		it._current = NULL;
		return it;
	}

	void erase( iterator & it )
	{
		Remove( it._current->pBucket->dwKey );
	}
	void clear()
	{
		RemoveAll();
		ASSERT( 0 == GetDataNum() );
	}
	
protected:
	HashList *				addList(HashBucket * pBucket)
	{
		HashList*	cur = NULL;
		//HashList*	prv = NULL;
		//HashList*	next = NULL;
		
		if (!m_pListHead)
		{
			m_pListCur = m_pListHead = m_pHashListPool->Alloc();
			m_pListCur->pPrv	= NULL;
			m_pListCur->pNext	= NULL;
			
			m_pListTail = m_pListCur;
			m_pListCur->pBucket = pBucket;
			pBucket->pList = m_pListHead;
			return m_pListHead;
		}
		//else
		{
			cur = m_pListTail->pNext = m_pHashListPool->Alloc();
			cur->pNext	= NULL;
			cur->pPrv	= m_pListTail;
			m_pListTail = cur;
			cur->pBucket = pBucket;
			pBucket->pList = cur;
			return cur;
		}
		//return NULL;
	}
	void				removeList(HashList * pList)
	{
		
	//	HashList*	cur = pList;
		HashList*	prv = pList->pPrv;
		HashList*	next = pList->pNext;
		
		
		if(pList == m_pListCur)		/// for iterator sync
			m_pListCur = next;
		
		if (prv)
			prv->pNext = next;
		else 
			m_pListHead = next;
		
		if (next)
			next->pPrv = prv;
		else
			m_pListTail = prv;
		
		m_pHashListPool->Free(pList); // delete pList;
		pList = NULL;	
	}	
};


} // namespace A

#endif //_SOLARHASHTABLE_H_INCLUDED_
