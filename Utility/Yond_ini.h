#ifndef __YOND_INI_INCLUDED_
#define __YOND_INI_INCLUDED_

#include "Utility.h"
using namespace std;

class Yond_ini
{ 
public:
	Yond_ini(const char* pIniFile);
	~Yond_ini(); 
	void Update();

	const char* 	GetString(const char* sect, const char* key, const char* defaultValue = "");
	void 		SetString(const char* sect, const char* key, const char* value);

	int 		GetInteger(const char* sect, const char* key, int defaultValue = 0);
	void 		SetInteger(const char* sect, const char* key, int value);

	double 		GetDouble(const char* sect, const char* key, double defaultValue = 0);
	void 		SetDouble(const char* sect, const char* key, double value);
    
	bool 		GetBool(const char* sect, const char* key, bool defaultValue = false);
	void 		SetBool(const char* sect, const char* key, bool value);
    
	void 		DeleteKey(const char* sect, const char* key);
	void 		DeleteSession(const char* sect);

	bool 		m_isOpenFile;
	
private: 
	string 					ini_file_name_; 
	map<string, string, less<string> > 	ini_map_; 
	
	Yond_ini(void) { m_isOpenFile = false; }
	
	bool OpenIni(const char* iniFile);
	bool WriteIni(const char* iniFile);
};

#endif //  __YOND_INI_INCLUDED_
