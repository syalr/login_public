#ifndef __DBCONNECTDIR_H_INCLUDED__
#define __DBCONNECTDIR_H_INCLUDED__

#include <Utility.h>
#include <dbCommon.h>

class IDBCInterface;

class DBConnect
{
public:
	DBConnect(void);
	virtual ~DBConnect(void);

	virtual BOOL Init();
	BOOL QueryDirect(QueryResult *);
	BOOL QueryLargeDataDirect(QueryResult *);
	
public:
	IDBCInterface *					m_pDBCInstance;
	fnCallBackMessage				m_fnErrorMessage;

};

class db_passport : public DBConnect
{
public:
	db_passport(void);
	virtual ~db_passport(void);
	
	virtual BOOL Init();
};

extern db_passport Obj_db_passport;

#endif 