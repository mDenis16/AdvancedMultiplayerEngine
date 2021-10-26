#include <Common.hpp>

#include "Server.hpp"

int main() {
	Server server;


	server.Initialize(22005);

	while(true)
	{
		Sleep(100000);

	}
}