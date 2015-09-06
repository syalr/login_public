#include "DBFactory.h"

DBFactory::DBFactory()
{
	m_pTempServerSessionPool = NULL;	
	m_pJsonServerSessionPool = NULL;
}

DBFactory::~DBFactory()
{
	if (m_pTempServerSessionPool) 	delete m_pTempServerSessionPool;
	if (m_pJsonServerSessionPool) 	delete m_pJsonServerSessionPool;
}

void DBFactory::Init()
{
	m_pTempServerSessionPool 	= new MemoryFactory<TempServerSession>;
	m_pJsonServerSessionPool 	= new MemoryFactory<JsonServerSession>; 
	m_pTempServerSessionPool->Initialize(1,1);
	m_pJsonServerSessionPool->Initialize(1,1); // ???
}

TempServerSession * DBFactory::AllocTempServerSession() 
{
	assert ( m_pTempServerSessionPool );
	TempServerSession * pSession = m_pTempServerSessionPool->Alloc();
	if ( pSession != NULL ) {
		pSession->Clear();
	}
	return pSession;
}

void DBFactory::FreeTempServerSession(TempServerSession * pServerSession) {
	return m_pTempServerSessionPool->Free(pServerSession);
}

JsonServerSession * DBFactory::AllocJsonServerSession() {
	assert ( m_pJsonServerSessionPool );
	if (m_pJsonServerSessionPool == NULL) {
		return NULL;
	}
	return m_pJsonServerSessionPool->Alloc();	
}
void DBFactory::FreeJsonServerSession(JsonServerSession * pServerSession) {
	return m_pJsonServerSessionPool->Free(pServerSession);
}

