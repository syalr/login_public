#ifndef _KEYV_FACTORY_H_INCLUDED_
#define _KEYV_FACTORY_H_INCLUDED_

#include <Network.h>
#include <Utility.h>
#include "LoginServerSession.h"
#include "DBServerSession.h"

using namespace A;

class JsonFactory : public Singleton<JsonFactory>
{
	
public:
	JsonFactory();
	~JsonFactory();

private:	
	MemoryFactory<LoginServerSession>	* m_pLoginServerSessionPool;
	MemoryFactory<DBServerSession>		* m_pDBServerSessionPool;

public:
	void Init(void);
	void Release(void);
	
	LoginServerSession * AllocLoginServerSession();
	void FreeLoginServerSession(LoginServerSession * pServerSession);
	
	DBServerSession * AllocDBServerSession();
	void FreeDBServerSession(DBServerSession * pServerSession);
};

#endif // _LINE_FACTORY_H_INCLUDED_
