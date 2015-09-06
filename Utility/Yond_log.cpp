#include "Yond_log.h"

static CLogMsg * g_pLogMsg = NULL;
Yond_mute g_mute;

Yond_log::Yond_log(void) {
}

Yond_log::~Yond_log(void) {
}

void Yond_log::Init(short p_size)
{
	g_pLogMsg = this;
	if ( m_logMenory.Initalize(p_size, 1) ) {
		m_firstLog == NULL;
	}
}

void Yond_log::AddLogMsg(int nType, char *pMsg)
{
	Yond_guard _guard(g_mute);
	if (pMsg==NULL && *pMsg=='\0'){
		return;
	}

	switch(nType)
	{
		case 1: { if (m_bDebug == false) return; }; 
		case 2: { if (m_bError == false) return; };
		case 3: { if (m_bInfo  == false) return; };
	}
	
	if ( m_lastLog != NULL ) {
		char * pMV = m_lastLog.szText;
		Yond_Zero(m_lastLog, sizeof(LOGITEM));
		if (shType == 0) {
			Yond_Strcat(pMV, "DEBUG ");
		}
		if (shType == 0) {
			Yond_Strcat(pMV, "ERROR ");
		}
		if (shType == 0) {
			Yond_Strcat(pMV, "INFO  ");				
		}
		PublicObject::SetCurrent(m_lastLog->szTime);
		Yond_Memcpy(m_lastLog.szText, pMsg);
		LOGITEM * _newLog = m_logMenory.Alloc();
		if ( _newLog == NULL ) {
			if ( m_firstLog == NULL ) {
				m_firstLog = _newLog;
				m_lastLog = _newLog;
			}
			else {
				m_lastLog->next = _newLog;
				m_lastLog = _newLog;		
			}
		}
	}
}

void Yond_log::ProcMsg()
{
	Yond_guard _guard(g_mute);
	if ( m_firstLog == NULL ) {
		return;
	} 
	
	LOGITEM * _newLog = m_firstLog->next;
	
	this->OutMsg(m_firstLog);
	m_logMenory.Free(m_firstLog);
	
	m_firstLog = _newLog;
}

BOOL Yond_log::OutMsg(LOGITEM * pMsg)
{
	// printf("%s：%s\n", item.szTime,item.szText);
	return TRUE;
}

void Yond_log_error(int nType, LPCSTR szMsg, ...) {
	
	if (szMsg!=NULL && szMsg=='\0')
		return;
	
	char pMsg[1024] = {0};
	
	va_list args;
	va_start(args, szMsg);
	vsnprintf(pMsg->szText, LOG_TEXT_L, szMsg, args);
	va_end(args);
	
	g_pLogMsg->AddLogMsg(nType, pMsg);
	return;
}

