#include "LoginFactory.h"

LoginFactory::LoginFactory()
{
	// Client ->
	m_pUserSessionPool			= NULL; 
	
	// Server ->
	m_pTempServerSessionPool	= NULL; 
	m_pBinarySessionPool		= NULL;
	m_pHttpSessionPool			= NULL;
	m_pJsonSessionPool			= NULL;
	m_pKeyvSessionPool			= NULL;
}                              

LoginFactory::~LoginFactory()
{
	// Client ->
	if (m_pUserSessionPool) 		delete m_pUserSessionPool;
	
	// Server ->
	if (m_pTempServerSessionPool) 	delete m_pTempServerSessionPool;
	if (m_pBinarySessionPool) 		delete m_pBinarySessionPool;
	if (m_pHttpSessionPool) 		delete m_pHttpSessionPool;
	if (m_pJsonSessionPool) 		delete m_pJsonSessionPool;
	if (m_pKeyvSessionPool) 		delete m_pKeyvSessionPool;
}

void LoginFactory::Init()
{
	// Client ->
	m_pUserSessionPool		= new MemoryFactory<UserSession>;
	m_pUserSessionPool->Initialize(999,1); 		// ???
	
	// Server ->
	m_pTempServerSessionPool	= new MemoryFactory<TempServerSession>;
	m_pBinarySessionPool		= new MemoryFactory<BinarySession>; 
	m_pHttpSessionPool			= new MemoryFactory<HttpSession>; 
	m_pJsonSessionPool			= new MemoryFactory<JsonSession>; 
	m_pKeyvSessionPool			= new MemoryFactory<KeyvSession>;
	m_pTempServerSessionPool->Initialize(1,1);
	m_pBinarySessionPool->Initialize(1,1);
	m_pHttpSessionPool->Initialize(1,1); 		// ???
	m_pJsonSessionPool->Initialize(1,1);
	m_pKeyvSessionPool->Initialize(1,1); 	// ???
}

// User
UserSession * LoginFactory::AllocUserSession() {
	assert ( m_pUserSessionPool );
	UserSession * pSession = m_pUserSessionPool->Alloc();
	if ( pSession != NULL ) {
		pSession->Release();
	}
	return pSession;
}
void LoginFactory::FreeUserSession(UserSession * pServerSession) {
	return m_pUserSessionPool->Free(pServerSession);
}

// Temp Server
TempServerSession * LoginFactory::AllocTempServerSession() {
	assert ( m_pTempServerSessionPool );
	TempServerSession * pSession = m_pTempServerSessionPool->Alloc();
	if ( pSession != NULL ) {
		pSession->Clear();
	}
	return pSession;
}
void LoginFactory::FreeTempServerSession(TempServerSession * pServerSession) {
	return m_pTempServerSessionPool->Free(pServerSession);
}

// Binary Server
BinarySession * LoginFactory::AllocBinarySession() {
	assert ( m_pBinarySessionPool );
	BinarySession * pSession = m_pBinarySessionPool->Alloc();
	if ( pSession != NULL ) {
		pSession->Release();
	}
	return pSession;
}
void LoginFactory::FreeBinarySession(BinarySession * pServerSession) {
	return m_pBinarySessionPool->Free(pServerSession);
}

// Http Server
HttpSession * LoginFactory::AllocHttpSession() {
	assert ( m_pHttpSessionPool );
	HttpSession * pSession = m_pHttpSessionPool->Alloc();
	if ( pSession != NULL ) {
		pSession->Release();
	}
	return pSession;
}
void LoginFactory::FreeHttpSession(HttpSession * pServerSession) {
	return m_pHttpSessionPool->Free(pServerSession);
}

// Json Server
JsonSession * LoginFactory::AllocJsonSession() {
	assert ( m_pJsonSessionPool );
	JsonSession * pSession = m_pJsonSessionPool->Alloc();
	if ( pSession != NULL ) {
		pSession->Release();
	}
	return pSession;
}
void LoginFactory::FreeJsonSession(JsonSession * pServerSession) {
	return m_pJsonSessionPool->Free(pServerSession);
}

// Keyv Server
KeyvSession * LoginFactory::AllocKeyvSession() {
	assert ( m_pKeyvSessionPool );
	KeyvSession * pSession = m_pKeyvSessionPool->Alloc();
	if ( pSession != NULL ) {
		pSession->Release();
	}
	return pSession;
}
void LoginFactory::FreeKeyvSession(KeyvSession * pServerSession) {
	return m_pKeyvSessionPool->Free(pServerSession);
}

