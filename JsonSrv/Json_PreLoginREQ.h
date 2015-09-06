#ifndef _JSON_PRELOGINREQ_H_INCLUDED_
#define _JSON_PRELOGINREQ_H_INCLUDED_

class Json_PreLoginREQ
{
	enum
	{
		eUNKNOWN 		= 0x01,
		eACCESSID		= 0x02,
		eUSERNAME 		= 0x04,
		ePASSWORD 		= 0x08,
		eSSHKEY 		= 0x10,
		eGAMEID 		= 0x20,
		ePROTOCOL 		= 0x40,
	};

public:

	Json_PreLoginREQ();
	~Json_PreLoginREQ();

	int ParseJson( const char * pInput );
	
	MSG_PRELOGIN_REQ * GetMsg(MSG_PRELOGIN_REQ * pMsg); 
	
private:

	MSG_PRELOGIN_REQ msg;
	
	// ...
	DWORD   dwStatus;
};



#endif // _JSON_STRING_H_INCLUDED_