#ifndef _LOGIN_FACTORY_H_INCLUDED_
#define _LOGIN_FACTORY_H_INCLUDED_

#include <Network.h>
#include <Utility.h>

#include "UserSession.h"

#include "TempServerSession.h"
#include "BinarySession.h"
#include "HttpSession.h"
#include "JsonSession.h"
#include "KeyvSession.h"

using namespace A;

class LoginFactory : public Singleton<LoginFactory>
{
public:

	LoginFactory();
	~LoginFactory();

private:
	
	// Client ->
	MemoryFactory<UserSession>			* m_pUserSessionPool;
	
	// Server ->
	MemoryFactory<TempServerSession>	* m_pTempServerSessionPool;  
	MemoryFactory<BinarySession>		* m_pBinarySessionPool;
	MemoryFactory<HttpSession>			* m_pHttpSessionPool;
	MemoryFactory<JsonSession>			* m_pJsonSessionPool;
	MemoryFactory<KeyvSession>			* m_pKeyvSessionPool;
	
public:

	void Init(void);
	void Release(void);

public:
	// User
	UserSession * AllocUserSession();
	void FreeUserSession(UserSession * pUser);
	
	// Temp Server
	TempServerSession * AllocTempServerSession();
	void FreeTempServerSession(TempServerSession * pServerSession);
	
	// Binary Server
	BinarySession * AllocBinarySession();
	void FreeBinarySession(BinarySession * pUser);
	
	// Http Server
	HttpSession * AllocHttpSession();
	void FreeHttpSession(HttpSession * pServerSession);
	
	// Json Server
	JsonSession * AllocJsonSession();
	void FreeJsonSession(JsonSession * pServerSession);
	
	// Keyv Server
	KeyvSession * AllocKeyvSession();
	void FreeKeyvSession(KeyvSession * pServerSession);
};

#endif
