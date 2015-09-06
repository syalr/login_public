#ifndef _JSON_PRELOGINANC_H_INCLUDED_
#define _JSON_PRELOGINANC_H_INCLUDED_

class Json_PreLoginANC
{
	
public:

	Json_PreLoginANC();
	~Json_PreLoginANC();

	void SetMsg(MSG_PRELOGIN_ANC * pMsg); 
	WORD GetJson(char * szJson, WORD wSize );
	
private:
	MSG_PRELOGIN_ANC msg;
};



#endif // _JSON_STRING_H_INCLUDED_