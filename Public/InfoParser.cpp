#include "InfoParser.h"

#define min(a,b) (((a) < (b)) ? (a) : (b))

InfoParser g_InfoParser;

InfoParser::InfoParser(void)
{
	pIniFile = NULL;
	//Init(".\\ServerInfo.ini");
}

InfoParser::~InfoParser(void)
{
	SAFE_DELETE(pIniFile);
}

// 初始化
BOOL InfoParser::Init( char *strFileName )
{
	pIniFile = new Yond_ini( strFileName );
	if (!pIniFile)
		return FALSE;

	if ( !pIniFile->m_isOpenFile ) {
		return FALSE;
	}

	return TRUE;
}

BOOL InfoParser::GetString( 
			const char * strSection, 
			const char * szName, 
			const char * szDefault, 
				  char * pszValue )
{
	char szBuffer[256];

	const char *pChar = NULL;
	
	pChar = pIniFile->GetString( strSection, szName, szDefault );
	if(!pChar)
		return FALSE;

	memset( szBuffer, 0x0, sizeof(szBuffer) );
	strncpy( szBuffer, pChar, min( 256, strlen(pChar) ) );
	
	int nLen = (int)strlen( szBuffer ); 
	if ( nLen > 0 )
	{
		strcpy( pszValue, szBuffer );
		return TRUE;
	}

	return FALSE;
}

int InfoParser::GetInt( char * strSection, char * szName, char * szDefault )
{	
	int retInt = 0;
	retInt = pIniFile->GetInteger( strSection, szName, -1 );
	return retInt;
}

// 获取服务器端口信息
SERVER_INFO InfoParser::GetServerInfo(eSERVER_TYPE type)
{
	std::map<eSERVER_TYPE, std::string> mapSrvNameList;
	mapSrvNameList[AGENT_SERVER]		= "AGENT_SERVER"; 	// 代理服务器
	mapSrvNameList[GAME_SERVER]			= "GAME_SERVER"; 	// 游戏服务器
	mapSrvNameList[DB_SERVER]			= "DB_SERVER"; 		// 数据库服务器
	mapSrvNameList[LOGIN_SERVER]		= "LOGIN_SERVER"; 	// 登陆服务器
	mapSrvNameList[WORLD_SERVER]		= "WORLD_SERVER"; 	// 聊天服务器
	
	//mapSrvNameList[WORLD_SERVER]		= "EXTRA_AGENT_SERVER"; 	// 额外的 Agent 服务器

	SERVER_INFO sInfo;
	std::map<eSERVER_TYPE, std::string>::iterator itSrv = mapSrvNameList.find(type);
	if (itSrv == mapSrvNameList.end())
	{
		return sInfo;
	}
	
	char szName[32] = {0};
	strcpy(szName, itSrv->second.c_str());
	GetString(szName, "IP", "", sInfo.m_strIp);
	sInfo.m_dwPort = GetInt(szName, "PORT", "0");

	//assert(0 != sInfo.m_dwPort);
	//assert(0 != strlen(sInfo.m_strIp));
	
	return sInfo;
}
