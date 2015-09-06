#ifndef __QUERY_EXECUTE_PROCESSOR_H_INCLUDED__
#define __QUERY_EXECUTE_PROCESSOR_H_INCLUDED__

#include <Utility.h>

#define __DISABLE_COPY(className)					\
		private:									\
			className( const className &);			\
			void operator = ( const className &)

class IDBCInterface;
class HyDatabase;
//=======================================================================================================================
/// 
/**
	@author
	@since
	@remarks
			- WakeUp 
			- QueryExecuteProcessor 
			- IDBCInterface->Execute(query)
	@note
			- QueryExecuteProcessor 
*/
//=======================================================================================================================
class QueryExecuteProcessor :	public Yond_thread
{
public:
	QueryExecuteProcessor( HyDatabase & psdb, IDBCInterface * pDBCInstance );
	virtual ~QueryExecuteProcessor(void);

	virtual VOID					EndThread();
	virtual DWORD					run();

private:
	HyDatabase &					m_psdb;

	IDBCInterface *					m_pDBCInstance;

	__DISABLE_COPY(QueryExecuteProcessor);
};


#endif // __QUERY_EXECUTE_PROCESSOR_H__
	
