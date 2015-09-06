#include "MsgProcess.h"

//MsgProcess g_MsgProcess;

MsgProcess::MsgProcess(void)
{
}

MsgProcess::~MsgProcess(void)
{
}

vector<string> MsgProcess::Split( char * pInput, const string &pattern )
{
	vector<string> resultVec;
	string tempStr(pInput);
	tempStr += pattern; // 扩展字符串以方便操作
	
	string::size_type nSize = tempStr.size();

	for( int i=0; i<nSize; ++i )
	{
		string::size_type pos = tempStr.find( pattern, i );
		if( ( pos != string::npos ) && ( pos < nSize ) ) {
			string itemStr = tempStr.substr( i, pos - i );
			resultVec.push_back( itemStr );			
			i = pos + pattern.size() - 1;
		}
	}
	
	return resultVec;
}

