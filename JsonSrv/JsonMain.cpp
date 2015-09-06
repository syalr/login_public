#include "JsonServer.h"


int main(int argc, char ** argv) 
{
	g_pJsonServer = new JsonServer;
	g_pJsonServer->Init();
	
	while(true) {
		Sleep(100);
		g_pJsonServer->Update(0);
	}
	
	return 0;
}