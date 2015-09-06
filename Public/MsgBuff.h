#ifndef __MsgBuff_H_INCLUDED_
#define __MsgBuff_H_INCLUDED_

#include <Utility.h>
#pragma pack(push,1)//结构成员1字节对齐

class CMsgBuff
{
public:
	CMsgBuff(void);
	CMsgBuff(void * pBuff, WORD wLen);

	~CMsgBuff(void);

public:
	VOID SetBuff(void * pBuff, WORD wLen);

	//写数据
	int		Write(INT nData);
	int		Write(BYTE byData);
	int		Write(WORD wData);
	int		Write(DWORD dwData);
	int		Write(double dbData);
	int		Write(FLOAT fData);
	int		Write(CHAR * strData);
	int		Write(void *pData, WORD wLen);


	//读数据
	int		Read(INT & nData);
	int		Read(BYTE & byData);
	int		Read(WORD & wData);
	int		Read(DWORD & dwData);
	int		Read(double & dbData);
	int		Read(FLOAT & fData);
	int		Read(CHAR * strData);
	int		Read(void *pData, WORD wLen);

	//获取写的数据长度
	int		GetWriteLen() { return m_wWriteLen; }

	//获取读的数据长度
	int		GetReadLen()  { return m_wReadLen; }

	//获取头地址
	BYTE * GetHead()	{ return m_pHead; }

	//获取当前地址
	BYTE * GetPost()	{ return m_pBuff; }


private:
	//Buff指针
	BYTE * m_pHead;
	BYTE * m_pBuff;

	//Buff总长度
	WORD m_wTotalLen;

	//Buff填充数据的长度
	WORD m_wWriteLen;

	//Buff读取数据的长度
	WORD m_wReadLen;

};

#endif

