#ifndef __MEMORYFACTORY_HPP_INCLUDED_
#define __MEMORYFACTORY_HPP_INCLUDED_

#include "Utility.h"

namespace A
{
	template <class DataType>
	class MemBand
	{
		DataType **		ppDataType;
		DataType *		pDeleteArray;
		DWORD			nMaxBandObjectNum;
		DWORD			FreeIdx;

	public:
		MemBand *	pPrev;
		MemBand *	pNext;

		MemBand():pPrev(NULL),pNext(NULL){}		
		virtual ~MemBand()
		{
			pPrev=NULL;pNext=NULL;
			delete [] (ppDataType);
			delete [] (pDeleteArray);
		}
		inline DWORD GetAvailableNumberOfObjects()
		{
			return FreeIdx;
		}
		inline DWORD GetMaxNumberOfObjects()
		{
			return nMaxBandObjectNum;
		}

		DataType * GetObject( DWORD dwIdx )
		{
			return &pDeleteArray[dwIdx];
		}

		static MemBand * AllocBand( MemBand<DataType> *& head, DWORD nMaxObject )
		{
			MemBand<DataType>* pp = new MemBand<DataType>;
			pp->ppDataType = new DataType*[nMaxObject];
			memset(pp->ppDataType, 0, sizeof(DataType*)*nMaxObject);
			pp->pDeleteArray = new DataType[nMaxObject];
			for(DWORD i = 0 ; i < nMaxObject ; i++)
			{
				pp->ppDataType[i] = &pp->pDeleteArray[i];
			}
			pp->nMaxBandObjectNum	= nMaxObject;
			pp->FreeIdx				= nMaxObject;
			pp->pNext				= head;
			if(head)
				head->pPrev			= pp;
			head = pp;

			return pp;
		}

		static void FreeBand( MemBand<DataType> * pDelBand )
		{
			//head node one call, all delete;
			MemBand<DataType> * db = pDelBand;
			while(db)
			{
				MemBand<DataType> * tmp = db;
				db = db->pNext;
				delete tmp;
			}
		}
		DataType * AlloObject()
		{
			//	TASSERT(FreeIdx-1 >= 0);
			if(--FreeIdx == -1)
			{
				FreeIdx = 0;
				return NULL;
			}
			else
				return ppDataType[FreeIdx];
		}
		BOOL FreeObject(DataType * data)
		{
			//	TASSERT(FreeIdx+1 < nMaxNum);
			if(FreeIdx  < nMaxBandObjectNum )
				ppDataType[FreeIdx++] = data;
			else
				return FALSE;
			return TRUE;
		}
	};

	template <class Type>
	class MemoryFactory
	{
	public:
		MemoryFactory()
			:	m_pBandHead( NULL ),
				m_pFreeBand( NULL ),
				m_dwPoolExtendSize( 0 )
		{
		}
		virtual ~MemoryFactory()
		{
			Release();
		}
		BOOL Initialize( DWORD dwPoolBasicSize, DWORD dwPoolExtendSize = 0 )
		{
			if( 0 == dwPoolExtendSize )
				m_dwPoolExtendSize = dwPoolBasicSize/2+1;
			else
				m_dwPoolExtendSize = dwPoolExtendSize;

			if( NULL == m_pBandHead )
			{
				MemBand<Type>::AllocBand( m_pBandHead, dwPoolBasicSize );
				m_pFreeBand = m_pBandHead;
				if( !m_pFreeBand ) 
					return FALSE;
				return TRUE;
			}
			return FALSE;
		}

		void Release()
		{
			if( m_pBandHead )
			{
				MemBand<Type>::FreeBand( m_pBandHead );
				//m_pBandHead->FreeBand();
				m_pBandHead = NULL;
				m_pFreeBand = NULL;
			}
		}
		
		inline Type * Alloc()
		{
			Type * pn = m_pFreeBand->AlloObject();
			if(pn == NULL)
			{
				if( !m_pFreeBand->pPrev )
				{
					MemBand<Type>::AllocBand( m_pBandHead, m_dwPoolExtendSize );
					m_pFreeBand = m_pBandHead;
					pn = m_pFreeBand->AlloObject();
				}
				else
				{
					m_pFreeBand = m_pFreeBand->pPrev;
					pn = m_pFreeBand->AlloObject();
				}
			}
			//ASSERT( !IsBadReadPtr( pn, sizeof(Type) ) );
			//ASSERT( !IsBadWritePtr( pn, sizeof(Type) ) );

			return pn;
		}
		inline void Free(Type * pNode)
		{
			//ASSERT( !IsBadReadPtr( pNode, sizeof(Type) ) );
			//ASSERT( !IsBadWritePtr( pNode, sizeof(Type) ) );

			if(!m_pFreeBand->FreeObject(pNode))
			{
				if( !m_pFreeBand->pNext )
				{
					printf( "!m_pFreeBand->pNext.\n" );
					return;
				}

				m_pFreeBand = m_pFreeBand->pNext;


				if(!m_pFreeBand->FreeObject(pNode))	//<
				{
				}
			}
		}

		inline DWORD GetPoolBasicSize()
		{
			return m_pBandHead->GetMaxNumberOfObjects();
		}

		inline DWORD GetPoolExtendSize()
		{
			return m_dwPoolExtendSize;
		}

		inline DWORD GetNumberOfBands()
		{
			DWORD dwTotalNum = 0;
			MemBand<Type> *	pHead = m_pBandHead;
			while( pHead )
			{
				++dwTotalNum;
				pHead = pHead->pNext;
			}
			return dwTotalNum;
		}

		inline DWORD GetAvailableNumberOfTypes()
		{
			DWORD dwTotalNum = 0;
			MemBand<Type> * pHead = m_pBandHead;
			while(pHead)
			{
				dwTotalNum += pHead->GetAvailableNumberOfObjects();
				pHead = pHead->pNext;
			}
			return dwTotalNum;
		}
	protected:
		MemBand<Type> *	m_pBandHead;
		MemBand<Type> *	m_pFreeBand;
		DWORD			m_dwPoolExtendSize;
	};

}

#endif //end __MEMORYFACTORY_HPP_INCLUDED_
	
	
	
