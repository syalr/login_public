#include "BinaryServer.h"


int main(int argc, char ** argv) 
{
	g_pBinaryServer = new BinaryServer;
	g_pBinaryServer->Init();
	
	while(true) {
		Sleep(100);
		g_pBinaryServer->Update(0);
	}
	
	return 0;
}