#include <Utility.h>
#include <iostream>
#include "Yond_json.h"
#include "JsonParser.h"

using std::cout;
using std::endl;

JsonParser::JsonParser()
{
	memset( this, 0, sizeof(this) );
}


JsonParser::~JsonParser()
{
	
}

int JsonParser::ParseJson( const char * pInput )
{
	dwStatus = eUNKNOWN;
	
	cJSON * root = NULL;
	
	root = cJSON_Parse( pInput );
	if ( root == NULL ) {
		return -1;
	}
	
	int nLen = 0;
	
	// protocol id
	cJSON * protocol = cJSON_GetObjectItem( root, "Protocol" );
	if ( protocol ) {
		dwStatus |= ePROTOCOL;
		dwProtocol = 0;
		dwProtocol = protocol->valueint;
		if ( dwProtocol == 0 )
		{
			char szProtocol[12];
			nLen = strlen( protocol->valuestring );
			if ( nLen < sizeof(szProtocol) )
			{
				memcpy( szProtocol, protocol->valuestring, nLen + 1 );
				szProtocol[ nLen ] = '\0';
				dwProtocol = atol(szProtocol);
			}
		}
	}
	
	if ( root ) {
		cJSON_Delete( root );
	}
	
	return 0;
}

DWORD JsonParser::GetProtocol()
{
	if ( dwStatus & ePROTOCOL )
	{
		return dwProtocol;
	}
	return 0;
}