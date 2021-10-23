#include <Common.hpp>

#include "Server.hpp"

int main() {
	Server server;


	server.Initialize(5555);

	while(true)
	{
		Sleep(100000);

	}
}