#ifndef _MSG_PROCESS_H_INCLUDED_
#define _MSG_PROCESS_H_INCLUDED_

#include <Common.h>
#include <Utility.h>

using namespace std;

class MsgProcess
{
public:
	MsgProcess(void);
	~MsgProcess(void);

	// 
	vector<string> Split( char * pInput, const string &pattern );
	
};

//extern MsgProcess g_MsgProcess;

#endif // _MSG_PROCESS_H_INCLUDED_
