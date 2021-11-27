#include <Common.hpp>

#include "Server.hpp"
#define _CRTDBG_MAP_ALLOC
int main() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	/*_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |
		_CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR,
		_CRTDBG_MODE_DEBUG);*/
#if NO_LAITY
	SetConsoleTextAttribute(hConsole,12);
	std::cout << "WARNING! Server was build in NO_LAITY version which means there is no one to test sync and you need to figure out yourself." << std::endl;
	SetConsoleTextAttribute(hConsole, 2);
#endif
	Server server;


	server.Initialize(22005);

	int i = 0;
	std::cin >> i;
}