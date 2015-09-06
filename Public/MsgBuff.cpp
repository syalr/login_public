#include "MsgBuff.h"


CMsgBuff::CMsgBuff(void)
{
	m_pHead = NULL;
	m_pBuff = NULL;

	m_wTotalLen = 0;
	m_wWriteLen = 0;
	m_wReadLen = 0;
}


CMsgBuff::~CMsgBuff(void)
{
}


CMsgBuff::CMsgBuff(void * pBuff, WORD wLen)
{
	//memset(pBuff,0x0,wLen);
	m_pHead = (BYTE *)pBuff;
	m_pBuff = (BYTE *)pBuff;

	m_wTotalLen = wLen;
	m_wWriteLen = 0;
	m_wReadLen = 0;

}


VOID CMsgBuff::SetBuff(void * pBuff, WORD wLen)
{
	m_pHead = (BYTE *)pBuff;
	m_pBuff = (BYTE *)pBuff;

	m_wTotalLen = wLen;
	m_wWriteLen = 0;
	m_wReadLen = 0;


}

//写数据
int	CMsgBuff::Write(INT nData)
{
	_ASSERT(m_pBuff);
	_ASSERT(m_wWriteLen+sizeof(INT)<=m_wTotalLen);

	memcpy(m_pBuff, &nData, sizeof(INT) );
	m_wWriteLen += sizeof(INT);
	m_pBuff += sizeof(INT);
	
	return m_wWriteLen;
}


int	CMsgBuff::Write(BYTE byData)
{
	_ASSERT(m_pBuff);
	_ASSERT(m_wWriteLen+sizeof(BYTE)<=m_wTotalLen);

	memcpy(m_pBuff, &byData, sizeof(BYTE) );
	m_wWriteLen += sizeof(BYTE);
	m_pBuff += sizeof(BYTE);

	return m_wWriteLen;
}

int	CMsgBuff::Write(WORD wData)
{
	_ASSERT(m_pBuff);
	_ASSERT(m_wWriteLen+sizeof(WORD)<=m_wTotalLen);

	memcpy(m_pBuff, &wData, sizeof(WORD) );
	m_wWriteLen += sizeof(WORD);
	m_pBuff += sizeof(WORD);

	return m_wWriteLen;
}

int	CMsgBuff::Write(DWORD dwData)
{
	_ASSERT(m_pBuff);
	_ASSERT(m_wWriteLen+sizeof(DWORD)<=m_wTotalLen);

	memcpy(m_pBuff, &dwData, sizeof(DWORD) );
	m_wWriteLen += sizeof(DWORD);
	m_pBuff += sizeof(DWORD);

	return m_wWriteLen;
}

int	CMsgBuff::Write(double dbData)
{
	_ASSERT(m_pBuff);
	_ASSERT(m_wWriteLen+sizeof(double)<=m_wTotalLen);

	memcpy(m_pBuff, &dbData, sizeof(double) );
	m_wWriteLen += sizeof(double);
	m_pBuff += sizeof(double);

	return m_wWriteLen;
}

int	CMsgBuff::Write(FLOAT fData)
{
	_ASSERT(m_pBuff);
	_ASSERT(m_wWriteLen+sizeof(FLOAT)<=m_wTotalLen);

	memcpy(m_pBuff, &fData, sizeof(FLOAT) );
	m_wWriteLen += sizeof(FLOAT);
	m_pBuff += sizeof(FLOAT);

	return m_wWriteLen;
}

int	CMsgBuff::Write(CHAR * strData)
{
	_ASSERT(m_pBuff);
	_ASSERT(m_wWriteLen+strlen(strData)+1<=m_wTotalLen);

	memcpy(m_pBuff, strData, strlen(strData)+1 );
	m_wWriteLen += strlen(strData)+1;
	m_pBuff += strlen(strData)+1;

	return m_wWriteLen;
}

int	CMsgBuff::Write(void *pData, WORD wLen)
{
	if(wLen <= 0)
		return 0;

	_ASSERT(m_pBuff);
	_ASSERT(m_wWriteLen+wLen<=m_wTotalLen);

	memcpy(m_pBuff, pData, wLen );
	m_wWriteLen += wLen;
	m_pBuff += wLen;

	return m_wWriteLen;
}


//读数据
int	CMsgBuff::Read(INT & nData)
{
	_ASSERT(m_pBuff);
	memcpy(&nData, m_pBuff, sizeof(INT));
	m_wReadLen += sizeof(INT);
	m_pBuff += sizeof(INT);

	return m_wReadLen;
}

int	CMsgBuff::Read(BYTE & byData)
{
	_ASSERT(m_pBuff);
	memcpy(&byData, m_pBuff, sizeof(BYTE));
	m_wReadLen += sizeof(BYTE);
	m_pBuff += sizeof(BYTE);

	return m_wReadLen;
}

int	CMsgBuff::Read(WORD & wData)
{
	_ASSERT(m_pBuff);
	memcpy(&wData, m_pBuff, sizeof(WORD));
	m_wReadLen += sizeof(WORD);
	m_pBuff += sizeof(WORD);

	return m_wReadLen;
}

int	CMsgBuff::Read(DWORD & dwData)
{
	_ASSERT(m_pBuff);
	memcpy(&dwData, m_pBuff, sizeof(DWORD));
	m_wReadLen += sizeof(DWORD);
	m_pBuff += sizeof(DWORD);

	return m_wReadLen;
}

int	CMsgBuff::Read(double & dbData)
{
	_ASSERT(m_pBuff);
	memcpy(&dbData, m_pBuff, sizeof(double));
	m_wReadLen += sizeof(double);
	m_pBuff += sizeof(double);

	return m_wReadLen;
}

int	CMsgBuff::Read(FLOAT & fData)
{
	_ASSERT(m_pBuff);
	memcpy(&fData, m_pBuff, sizeof(FLOAT));
	m_wReadLen += sizeof(FLOAT);
	m_pBuff += sizeof(FLOAT);

	return m_wReadLen;
}

int	CMsgBuff::Read(CHAR * strData)
{
	_ASSERT(m_pBuff);
	int nStrLen = 0;
	nStrLen = strlen((char * )m_pBuff)+1;
	memcpy(strData, m_pBuff, nStrLen );
	m_wReadLen += nStrLen;
	m_pBuff += nStrLen;

	return m_wReadLen;
}

int	CMsgBuff::Read(void *pData, WORD wLen)
{
	_ASSERT(m_pBuff);
	memcpy(pData, m_pBuff, wLen);
	m_wReadLen += wLen;
	m_pBuff += wLen;

	return m_wReadLen;

}