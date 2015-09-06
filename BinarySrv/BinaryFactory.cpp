#include "BinaryFactory.h"

BinaryFactory::BinaryFactory()
{
	m_pLoginServerSessionPool 	= NULL;
	m_pDBServerSessionPool 		= NULL;
}

BinaryFactory::~BinaryFactory()
{
	if (m_pLoginServerSessionPool) 	delete m_pLoginServerSessionPool;
	if (m_pDBServerSessionPool) 	delete m_pDBServerSessionPool;
}

void BinaryFactory::Init()
{
	m_pLoginServerSessionPool 	= new MemoryFactory<LoginServerSession>;
	m_pDBServerSessionPool 		= new MemoryFactory<DBServerSession>;
	
	m_pLoginServerSessionPool->Initialize(1,1);
	m_pDBServerSessionPool->Initialize(1,1);
}

LoginServerSession * BinaryFactory::AllocLoginServerSession() {
	assert(m_pDBServerSessionPool == NULL);
	return m_pLoginServerSessionPool->Alloc();
}
void BinaryFactory::FreeLoginServerSession(LoginServerSession * pServerSession) {
	return m_pLoginServerSessionPool->Free(pServerSession);
}

DBServerSession * BinaryFactory::AllocDBServerSession() {
	assert(m_pDBServerSessionPool == NULL);
	return m_pDBServerSessionPool->Alloc();
}
void BinaryFactory::FreeDBServerSession(DBServerSession * pServerSession) {
	return m_pDBServerSessionPool->Free(pServerSession);
}
