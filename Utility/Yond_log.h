#ifndef __Yond_LOG_H_INCLUDED__
#define __Yond_LOG_H_INCLUDED__

#include "Utility.h"
using namespace A;

#define LOG_DEBUG	1		
#define	LOG_ERROR	2
#define	LOG_INFO	3

#define	LOG_TIME_L	32
#define	LOG_TEXT_L	256

struct LOGITEM
{
	char szTime[LOG_TIME_L];
	char szText[LOG_TEXT_L];
	LOGITEM * next;
};

class Yond_log
{
public:
	Yond_log(void);
	~Yond_log(void);

public:
	void Init(short p_size);

	void AddLogMsg(LOGITEM * pMsg);
	
	void ProcMsg();

	virtual BOOL OutMsg(LOGITEM * pMsg);

public:
	static bool m_bDebug;	
	static bool m_bError;
	static bool m_bInfo;

private:
	LOGITEM * m_firstLog;
	LOGITEM * m_lastLog;
	
	MemoryFactory<LOGITEM> m_logMenory;	
};

void Yond_log_debug(int nType, LPCSTR szMsg, ...);
void Yond_log_error(int nType, LPCSTR szMsg, ...);
void Yond_log_info(int nType, LPCSTR szMsg, ...);

#endif
