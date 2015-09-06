#include "JsonFactory.h"

JsonFactory::JsonFactory()
{
	m_pLoginServerSessionPool 	= NULL;
	m_pDBServerSessionPool 		= NULL;
}

JsonFactory::~JsonFactory()
{
	if (m_pLoginServerSessionPool) 	delete m_pLoginServerSessionPool;
	if (m_pDBServerSessionPool) 	delete m_pDBServerSessionPool;
}

void JsonFactory::Init()
{
	m_pLoginServerSessionPool 	= new MemoryFactory<LoginServerSession>;
	m_pDBServerSessionPool 		= new MemoryFactory<DBServerSession>;
	
	m_pLoginServerSessionPool->Initialize(1,1);
	m_pDBServerSessionPool->Initialize(1,1);
}

LoginServerSession * JsonFactory::AllocLoginServerSession() {
	assert(m_pLoginServerSessionPool);
	return m_pLoginServerSessionPool->Alloc();
}
void JsonFactory::FreeLoginServerSession(LoginServerSession * pServerSession) {
	return m_pLoginServerSessionPool->Free(pServerSession);
}

DBServerSession * JsonFactory::AllocDBServerSession() {
	assert(m_pDBServerSessionPool);
	return m_pDBServerSessionPool->Alloc();
}
void JsonFactory::FreeDBServerSession(DBServerSession * pServerSession) {
	return m_pDBServerSessionPool->Free(pServerSession);
}
