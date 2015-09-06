#ifndef _INFOPARSER_H_INCLUDED_
#define _INFOPARSER_H_INCLUDED_

#include <Common.h>
#include <Network.h>
#include <Utility.h>
#include <Public.h>
using namespace std;

class InfoParser
{
public:
	InfoParser(void);
	~InfoParser(void);

	// 初始化
	BOOL	Init(char * strFileName);

	BOOL	GetString( 
				const char * strSection, 
				const char * szName, 
				const char * szDefault, 
					  char * pszValue );
				
	int 	GetInt( char * strSection, char * szName, char * szDefault );

	// 获取服务器端口信息
	SERVER_INFO GetServerInfo(eSERVER_TYPE type);

private:
	Yond_ini * pIniFile;	
};

extern InfoParser g_InfoParser;

#endif // _INFOPARSER_H_INCLUDED_
