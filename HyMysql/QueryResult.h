#ifndef __QUERYRESULT_H_INCLUDED__
#define __QUERYRESULT_H_INCLUDED__

#include <Utility.h>
#include "HyDatabaseDefine.h"

#define	MAX_QUERY_LENGTH	/*256*/512 

#pragma pack(push,1)

enum eQUERYTYPE
{
	eErrorQuery,
	eGeneralQuery,
	eLargeQuery,
};

class QueryResult
{	
public:
	QueryResult(){}
	virtual ~QueryResult(){}

	//
	virtual void					AllocData() {;}
	virtual void					InitData() {;}
	virtual void					dealData() {;}
	//

	virtual inline ULONG			GetParameterNum() { return 0; }
	virtual inline eQUERYTYPE		GetQueryType() { return eGeneralQuery; }
	virtual inline VOID				GetParameterInfo( ULONG ulParamIndex, SHORT & ioType, BYTE *& pParamPtr, LONG & lSize, SHORT & sqlType, SHORT & cType )  
	{
		__UNUSED(ulParamIndex);
		__UNUSED(ioType);
		__UNUSED(pParamPtr);
		__UNUSED(lSize);
		__UNUSED(sqlType);
		__UNUSED(cType);
	}

	virtual inline ULONG			GetMaxRowNum() = 0;
	virtual inline ULONG			GetMaxColNum() = 0;
	virtual inline BYTE *			GetBuffer( ULONG ulRow ) = 0;

	virtual inline VOID				GetColumnInfo( ULONG row, ULONG col, ULONG *& pColPtr, LONG & ulSize, SHORT & sColType ) 
	{
		__UNUSED(row);
		__UNUSED(col);
		__UNUSED(pColPtr);
		__UNUSED(ulSize);
		__UNUSED(sColType);
	};

	virtual inline ULONG *			GetColumnLengthPtr( ULONG col ) { __UNUSED(col); return NULL;	}

	inline DWORD					GetIndex() { return m_dwIndex;	}

	inline ULONG *					GetResultColNumPtr() { return &m_ulResultColNum;	}
	inline ULONG *					GetResultRowNumPtr() { return &m_ulResultRowNum;	}
	inline LONG *					GetAffectNumPtr() { return &m_lAffectNum;	}

	inline ULONG					GetResultColNum() { return m_ulResultColNum;	}
	inline ULONG					GetResultRowNum() { return m_ulResultRowNum;	}
	
	inline LONG						GetAffectNum() { return m_lAffectNum;	}

	inline VOID						SetAffectNum( LONG ulVal ) { m_lAffectNum = ulVal;	}
	inline VOID						SetResultColNum( ULONG ulVal ) { m_ulResultColNum = ulVal;	}
	inline VOID						SetResultRowNum( ULONG ulVal ) { m_ulResultRowNum = ulVal;	}
	inline VOID						SetIndex( DWORD dwIdx ) { m_dwIndex = dwIdx; }
	
	inline VOID *					GetVoidObject() { return m_pVoidObject;	}
	inline VOID						SetVoidObject( VOID * pVoidObject ) { m_pVoidObject = pVoidObject; }

	inline VOID						SetQuery( char * pszQuery ) { strncpy( m_pszQuery, pszQuery, MAX_QUERY_LENGTH ); }
	inline const char * const		GetQuery() { return m_pszQuery;	}
	
private:
	DWORD							m_dwIndex;
	VOID *							m_pVoidObject;
	ULONG							m_ulResultColNum;
	ULONG							m_ulResultRowNum;
	LONG							m_lAffectNum;
	char							m_pszQuery[MAX_QUERY_LENGTH];

};

#pragma pack(pop)

#endif // __QUERY_RESULT_H__
	
	
