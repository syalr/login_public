#ifndef _COMMSTRUCT_H_INCLUDED_
#define _COMMSTRUCT_H_INCLUDED_


struct SERVER_INFO
{
	char 	m_strIp[IP_MAX_LEN]; 	// IP
	DWORD 	m_dwPort; 				// PORT
	
	SERVER_INFO()
	{
		memset(this, 0, sizeof(SERVER_INFO));
		strcpy(m_strIp, "");
		m_dwPort = 0;		
	}	
};


#endif // _COMMSTRUCT_H_INCLUDED_
