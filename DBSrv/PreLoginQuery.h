#ifndef __PRELOGIN_QUERY_H_INCLUDED__
#define __PRELOGIN_QUERY_H_INCLUDED__

#include <dbCommon.h>

#pragma pack(push,1)

class Query_PreLogin : public QueryResult
{
	_DECLARE_QUERY_POOL( Query_PreLogin );
	
public:
	
	enum
	{
		RESULT_COL_NUM = 4,
		PARAM_COL_NUL = 6,
	};
	
	struct sRESULT {
		int 		m_nErrCode;
		unsigned int m_uiUserID;
		unsigned int m_uiPort;
		char m_szIP[16];
		
		sRESULT() 
		{
			memset( this, 0, sizeof(sRESULT) );
		}
	};
	
	ULONG uLength[RESULT_COL_NUM]; // 4
	
	vector<sRESULT> vctRes;
	
	void AllocData()
	{
		sRESULT m_RESULT;
		memset(&m_RESULT, 0, sizeof( m_RESULT) );
		vctRes.push_back( m_RESULT );
	}
	
	void InitData() { vctRes.clear(); }
	void dealData() { vctRes.pop_back(); }
	
	_BEGIN_BINDING_DATA( sRESULT, vctRes, uLength, 1, RESULT_COL_NUM)
		_BINDING_COLUMN(0, m_nErrCode)
		_BINDING_COLUMN(1, m_uiUserID)
		_BINDING_COLUMN(2, m_uiPort)
		_BINDING_COLUMN(3, m_szIP)
	_END_BINDING_DATA()
	
	struct tag_LOGIN {
		BYTE m_byUsername[32];
		BYTE m_byPassword[32];
		BYTE m_byAsshKey[33];
		BYTE m_byUsshKey[33];
		int  m_nGameid;
		int  m_nLoginType;
	} pParam[1];
	
	_BEGIN_PARAM_DATA(tag_LOGIN, pParam, PARAM_COL_NUL)
		_BINDING_PARAM_PTR(0, m_byUsername,  SQL_PARAM_INPUT)
		_BINDING_PARAM_PTR(1, m_byPassword,  SQL_PARAM_INPUT)
		_BINDING_PARAM_PTR(2, m_byAsshKey,  SQL_PARAM_INPUT)
		_BINDING_PARAM_PTR(3, m_byUsshKey,  SQL_PARAM_INPUT)
		_BINDING_PARAM_PTR(4, m_nGameid,  SQL_PARAM_INPUT)
		_BINDING_PARAM_PTR(5, m_nLoginType,  SQL_PARAM_INPUT)
	_END_PARAM_DATA()
};

#pragma pack(pop)

#endif // __PRELOGIN_QUERY_H_INCLUDED__