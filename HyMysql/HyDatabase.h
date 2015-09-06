#ifndef __HYDATABASE_H_INCLUDED__
#define __HYDATABASE_H_INCLUDED__

#include "HyDatabaseDefine.h"
#include <Utility.h>

class QueryResult;
class IDBCInterface;

//typedef std::list<QueryResult *>		QUERYRESULT_LIST;
//typedef QUERYRESULT_LIST::iterator		QUERYRESULT_ITER;

class HyDatabase
{
	friend class						QueryExecuteProcessor;
	
public:
	HyDatabase(void);
	~HyDatabase(void);

	BOOL								Initialize( HyDatabaseDesc & desc );
	void								Update();
	void								Release();

	BOOL								DBQuery( QueryResult * pQuery );

	DWORD								GetBufferedQueryNum() { return m_pQueryList->GetCount(); }
private:

	inline	void						SwitchQueryResultList() {	
										CLinkedlist<QueryResult *> * pTmpList = m_pQueryResultPushList; m_pQueryResultPushList = m_pQueryResultPopList; m_pQueryResultPopList = pTmpList;	}

	IDBCInterface **					m_ppDBCInstance;
	Yond_thread_pool *					m_pThreadPool;
	fnCallBackMessage					m_fnErrorMessage;
	fnCallBackQueryResult				m_fnQueryResult;
	WORD								m_wQueryProcessorNum;
	BOOL								m_bEndProcess;

	Yond_cond							m_condWakeup;
	Yond_mutex							m_lockWakeup;	

	Yond_mutex *						m_pQueryCS;
	Yond_mutex *						m_pQueryResultCS;
	CLinkedlist<QueryResult *> *		m_pQueryList;
	CLinkedlist<QueryResult *> *		m_pQueryResultPushList;
	CLinkedlist<QueryResult *> *		m_pQueryResultPopList;
	//QUERYRESULT_LIST *				m_pQueryResultPushList;
	//QUERYRESULT_LIST *				m_pQueryResultPopList;
	DWORD								m_dwDummy;
};


#endif // __HYDATABASE_H_INCLUDED__
	
	
