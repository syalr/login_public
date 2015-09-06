/*
*
*/

#ifndef __FUNCTION_MAP_H_INCLUDED__
#define __FUNCTION_MAP_H_INCLUDED_

#include "Utility.h"

#if defined(WIN32) || defined(WIN64)
	#include <hash_map>
	using namespace stdext;
#else
	#include <ext/hash_map>
	using  namespace   __gnu_cxx;
	#define MAKELONG(a, b)      ((LONG)(((WORD)((DWORD_PTR)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD_PTR)(b) & 0xffff))) << 16))
#endif

namespace A {
		
	class BASE_FUNC
	{
	public:
		BASE_FUNC(){}
		virtual ~BASE_FUNC(){}

		DWORD m_dwFunctionKey;
	};

	class FunctionMap
	{
		typedef hash_map< DWORD, BASE_FUNC * >	MAP_FUNCTION;
		typedef MAP_FUNCTION::iterator					MAP_FUNCTION_IT;
		typedef MAP_FUNCTION::value_type				MAP_FUNCTION_VT;

	protected:
		MAP_FUNCTION							m_mFunction; //
		MAP_FUNCTION_IT							m_itr;
	public:
		
		FunctionMap(){}
		~FunctionMap()
		{
			BASE_FUNC * pFunc;
			MAP_FUNCTION_IT itr;
			MAP_FUNCTION_IT itr_bk;
			for(itr = m_mFunction.begin(); itr != m_mFunction.end();)
			{
				pFunc = itr->second;
				if(pFunc)
				{
					delete pFunc;
					pFunc = NULL;
				}
				itr_bk = itr;
				itr++;
				m_mFunction.erase(itr_bk);
				//itr = m_mFunction.erase(itr);
			}
			m_mFunction.clear();
		}

		VOID SetFirst()
		{
			m_itr = m_mFunction.begin();
		}
		BOOL GetNext( BASE_FUNC *& pFuncOut )
		{
			++m_itr;
			pFuncOut = m_itr->second;
			if( m_itr != m_mFunction.end() )
				return TRUE;
			return FALSE;
		}

		BOOL  Add( BASE_FUNC * pFunction )
		{
			MAP_FUNCTION_IT			it;
			BASE_FUNC *				pTemp = NULL;

			if( !m_mFunction.empty() )
			{
				it = m_mFunction.find( pFunction->m_dwFunctionKey );
				if(it != m_mFunction.end())	pTemp = it->second;
			}

			if( pTemp != NULL ) 
			{
				return FALSE;
			}

			m_mFunction.insert(MAP_FUNCTION_VT( pFunction->m_dwFunctionKey, pFunction ));
			return TRUE;
		}

		BASE_FUNC * Find(const DWORD dwFunctionKey)
		{
			MAP_FUNCTION_IT			it;
			BASE_FUNC *				pTemp = NULL;


			if( !m_mFunction.empty() )
			{
				it = m_mFunction.find( dwFunctionKey );
				if(it != m_mFunction.end())	pTemp = it->second;
			}

			return pTemp;
		}
	};

};

#endif // __FUNCTION_MAP_H__
	
