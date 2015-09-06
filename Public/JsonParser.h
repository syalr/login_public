#ifndef _JSONPARSER_H_INCLUDED_
#define _JSONPARSER_H_INCLUDED_

class JsonParser
{
	enum
	{
		eUNKNOWN 		= 0x01,
		ePROTOCOL 		= 0x02,
	};

public:
	JsonParser();
	~JsonParser();

	int ParseJson( const char * pInput );
	
	DWORD GetProtocol();
	
private:
	
	DWORD   dwProtocol;
	
	DWORD   dwStatus;
};



#endif // _JSON_STRING_H_INCLUDED_