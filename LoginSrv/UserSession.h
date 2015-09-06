#ifndef _UserSession_H_INCLUDED_
#define _UserSession_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "ServerSession.h"

enum eUSER_TYPE 
{ 
	UT_TEMP_USER = 1, 		// 临时性用户
	UT_BINARY_USER,
	UT_KEYV_USER,
	UT_HTTP_USER,
	UT_JSON_USER,
};

class UserSession : public NetworkObject
{
public:
	UserSession();
	virtual ~UserSession();
	
	virtual BOOL Update( DWORD dwDeltaTick );
	virtual BOOL SendPacket(BYTE *pMsg, WORD wSize);

	// UserType
	virtual eUSER_TYPE GetUserType() { return m_eUserType; }
	virtual void SetUserType(eUSER_TYPE eType) { m_eUserType = eType; }
	
	// UserKey = Accept Port;
	WORD GetUserKey() const;
	void SetUserKey(WORD dwKey);
	
	void CloseSession();
	
public:
	virtual void 	Init();
	virtual void 	Release();
	virtual void	OnAccept( DWORD dwNetworkIndex );
	virtual void	OnDisconnect();
	virtual void	OnRecv(BYTE *pMsg, WORD wSize);
	virtual void	OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual void	OnLogString( char * pszLog);
	
private:
	eUSER_TYPE 		m_eUserType;
	BOOL            m_bFirst;
	WORD 			m_wUserKey;
	
public:
	// static DWORD    m_byCloseDelay; 
	static DWORD    m_dwClientDelay;  
	static DWORD    m_dwServerDelay;
	DWORD 			m_dwOvertime;  // 超时时间
};

#endif // _AGENTPLAYER_H_INCLUDED_
