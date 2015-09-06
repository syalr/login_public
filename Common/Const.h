#ifndef _CONST_H_INCLUDED_
#define _CONST_H_INCLUDED_

// 服务器类别
enum eSERVER_TYPE
{
	UNKNOWN_SERVER 	 = 0,
	
	AGENT_SERVER		, 		// 代理服务器
	GAME_SERVER 		,		// 游戏服务器
	DB_SERVER		,		// 数据库服务
	
	LOGIN_SERVER 		,		// 登录服务器
	KEYV_SERVER		,       	// KEYV服务器
	JSON_SERVER		,		// JSON服务器

	LINE_SERVER 		,		// 队列服务器
	WORLD_SERVER 		,		// 聊天服务器
	TEMP_SERVER 		,		// 临时
};


#endif // _CONST_H_INCLUDED_
