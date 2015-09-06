#ifndef _DB_FACTORY_H_INCLUDED_
#define _DB_FACTORY_H_INCLUDED_

#include <Network.h>
#include <Utility.h>

#include "ServerSession.h"
#include "TempServerSession.h"
#include "JsonServerSession.h"

using namespace A;

class DBFactory : public Singleton<DBFactory>
{
	
public:
	DBFactory();
	~DBFactory();

private:
	MemoryFactory<TempServerSession>	* m_pTempServerSessionPool;
	MemoryFactory<JsonServerSession>	* m_pJsonServerSessionPool; 

public:
	void Init(void);
	void Release(void);

	// Server
	TempServerSession * AllocTempServerSession();
	void FreeTempServerSession(TempServerSession * pServerSession);
	
	JsonServerSession * AllocJsonServerSession();
	void FreeJsonServerSession(JsonServerSession * pServerSession);
};

#endif // _DB_FACTORY_H_INCLUDED_
