#ifndef _PROTOCOL_H_INCLUDED_
#define _PROTOCOL_H_INCLUDED_

/*
 *	消息命名规则
 *	1) (Request)						_REQ 请求消息
 *	2) (Answer)							_ANS 应答消息
 *	3) Client -> Server (Sync)			_SYN 同步消息
 *	4) Server -> Client (Command)		-CMD 命令消息
 *	5) Server -> Client Broadcasting	_BRD 广播消息
 *	6) Server -> Client (Notify)		_NTF 服务器通知消息
 *	7) 									_ACK 允许
 *	8) 									_NAK 失败
*/

enum eCATEGORY_TYPE 
{
	Connect_Protocol    = 600, 		// 连接类型数据	(连接认证、心跳包)
	Login_Protocol		= 700,		// 登陆类型数据	(预登录、登录、退出)
	Games_Protocol		= 800,		// 游戏类型数据
	Update_Protocol		= 900,		// 更新类型数据 (查询 和 插入数据类型)
};

#endif // _PROTOCOL_H_INCLUDED_
