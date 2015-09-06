//#include "stdafx.h"
#include "Yond_ini.h"

typedef map<string, string, less<string> > strMap; 
typedef strMap::iterator strMapIt; 

using namespace std; 

const char* const MIDDLESTRING = "____****____"; 

struct IniAnalyze 
{ 
	string strSect; 
	strMap *pMap; 
	IniAnalyze(strMap & strMap):pMap(&strMap){}; 

	//重载() 
	void operator()(const string &sIni) 
	{ 
		string::size_type first = sIni.find('['); 
		string::size_type last = sIni.rfind(']');
		if( first != string::npos && last != string::npos && first != last + 1) 
		{ 
			strSect = sIni.substr(first + 1, last - first - 1); 
			return ; 
		} 

		if(strSect.empty()) 
		{ 
			return ; 
		} 

		if((first = sIni.find('=')) == string::npos) 
		{ 
			return ; 
		} 

		string strtmp1 = sIni.substr(0, first); 
		string strtmp2 = sIni.substr(first + 1, string::npos); 
		first= strtmp1.find_first_not_of(" \t"); 
		last = strtmp1.find_last_not_of(" \t"); 

		if(first == string::npos || last == string::npos) 
		{ 
			return ; 
		} 

		string strkey = strtmp1.substr(first, last - first + 1); 
		first = strtmp2.find_first_not_of(" \t"); 

		if(((last = strtmp2.find("\t#", first)) != string::npos) || 
			((last = strtmp2.find(" #", first)) != string::npos) || 
			((last = strtmp2.find("\t;",first)) != string::npos) || 
			((last = strtmp2.find(" ;", first)) != string::npos) || 
			((last = strtmp2.find("\t//", first)) != string::npos)|| 
			((last = strtmp2.find(" //",  first)) != string::npos)||
			((last = strtmp2.find("\r",  first)) != string::npos) ||
			((last = strtmp2.find("\n",  first)) != string::npos)) 
		{ 
			strtmp2 = strtmp2.substr(0, last - first + 1); 
		} 

		last = strtmp2.find_last_not_of(" \t"); 

		if(first == string::npos || last == string::npos) 
		{ 
			return ; 
		} 

		string value = strtmp2.substr(first, last - first + 1); 
		string mapkey = strSect + MIDDLESTRING + strkey; 
		(*pMap)[mapkey] = value; 
	} 
}; 

Yond_ini::Yond_ini(const char* iniFile) 
{ 
	ini_file_name_ = iniFile;

	m_isOpenFile = OpenIni(iniFile); 
} 

Yond_ini::~Yond_ini() 
{ 
} 

void Yond_ini::Update() 
{ 
	WriteIni(ini_file_name_.c_str()); 
} 

const char* Yond_ini::GetString(const char* sect, const char* key, const char* defaultValue) 
{ 
	string mapkey = sect; 
	mapkey += MIDDLESTRING; 
	mapkey += key; 

	strMapIt it = ini_map_.find(mapkey); 
	if(it == ini_map_.end()) 
	{ 
		return defaultValue; 
	} 

	return it->second.c_str(); 
} 

void Yond_ini::SetString(const char* sect, const char* key, const char* value) 
{ 
	string mapkey = sect; 
	mapkey += MIDDLESTRING; 
	mapkey += key; 

	strMapIt it = ini_map_.find(mapkey); 
	if(it != ini_map_.end()) 
	{ 
		it->second = value; 
		return ; 
	} 

	ini_map_[mapkey] = value; 
} 

int Yond_ini::GetInteger(const char* sect, const char* key, int iValue ) 
{ 
	const char *value = GetString(sect, key, NULL); 
	if (NULL == value) 
	{ 
		return iValue; 
	} 
	return atoi(value); 
} 

void Yond_ini::SetInteger(const char* sect, const char* key, int iValue) 
{ 
	char szValue[25] = {0}; 
	sprintf(szValue, "%d", iValue); 
	SetString(sect, key, szValue); 
} 

double Yond_ini::GetDouble(const char* sect, const char* key, double dValue ) 
{ 
	const char *value = GetString(sect, key, NULL); 
	if (NULL == value) 
	{ 
		return dValue; 
	} 
	return atof(value); 
} 

void Yond_ini::SetDouble(const char* sect, const char* key, double dValue) 
{ 
	char szValue[25] = {0}; 
	sprintf(szValue, "%f", dValue); 
	SetString(sect, key, szValue); 
} 

bool Yond_ini::GetBool(const char* sect, const char* key, bool bValue ) 
{ 
	const char *value = GetString(sect, key, NULL); 
	if (NULL == value) 
	{ 
		return bValue; 
	} 
	return strcmp(value, "1") == 0? true : false; 
} 

void Yond_ini::SetBool(const char* sect, const char* key, bool bValue) 
{ 
	char szValue[2] = {0}; 
	szValue[0] = '0'; 
	if (bValue) szValue[0] = '1'; 
	SetString(sect, key, szValue); 
} 

void Yond_ini::DeleteKey(const char* sect, const char* key) 
{ 
	string mapkey = sect; 
	mapkey += MIDDLESTRING; 
	mapkey += key; 

	strMapIt it = ini_map_.find(mapkey); 
	if(it != ini_map_.end()) 
	{ 
		ini_map_.erase(it); 
	} 
} 

void Yond_ini::DeleteSession(const char* sect) 
{ 
	strMapIt it; 
	string sSess, strSect; 
	unsigned int uPos = 0; 
	for (it = ini_map_.begin(); it != ini_map_.end(); ++it) 
	{ 
		strSect = it->first; 
		uPos = strSect.find(MIDDLESTRING); 
		sSess = strSect.substr(0, uPos); 
		if (sSess == sect) 
		{ 
			ini_map_.erase(it); 
		} 
	} 
} 


bool Yond_ini::OpenIni(const char* iniFile) 
{ 
	FILE* fp = fopen(iniFile, "rb");

	if(fp == NULL) 
	{ 
		return false; 
	} 

	vector<string> strVect; 

	char strLine[512]; 
	while(!feof(fp)) 
	{ 
		char *s = fgets(strLine, 512, fp);		
		if ( s != NULL )
		{			
			strVect.push_back(strLine); 
		}
	} 
	fclose(fp); 

	if(strVect.empty()) 
	{ 
		return false; 
	} 

	for_each(strVect.begin(), strVect.end(), IniAnalyze(ini_map_)); 

	return !ini_map_.empty(); 
} 

bool Yond_ini::WriteIni(const char* iniFile) 
{ 
	if (ini_map_.empty()) 
	{ 
		return false; 
	} 

	FILE* fp = fopen(iniFile, "wb");
	if (fp == NULL) 
	{ 
		return false; 
	} 

	strMapIt it; 
	int iMID_LEN = strlen(MIDDLESTRING); 
	string sSessSave = "", sSess, sKey, strSect; 
	unsigned int uPos = 0; 
	for (it = ini_map_.begin(); it != ini_map_.end(); ++it) 
	{ 
		strSect = it->first; 
		uPos = strSect.find(MIDDLESTRING); 
		sSess = strSect.substr(0, uPos); 
		if (sSessSave != sSess) 
		{ 
			sSessSave = sSess; 			
			fprintf(fp, "[%s]\n", sSess.c_str());
		} 

		sKey = strSect.substr(uPos + iMID_LEN, strlen(strSect.c_str()) - uPos - iMID_LEN); 		
		fprintf(fp, "%s = %s\n", sKey.c_str(), it->second.c_str());
	} 

	fclose(fp); 

	return true; 
} 