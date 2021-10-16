#pragma once

#include <Common.hpp>
#include <Shared.hpp>
#include "Network.h"


class Server
{
public:
	Server();
	~Server();

	void Initialize(std::uint32_t port);

	Network GameNetwork;

	
private:

};
