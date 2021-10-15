#pragma once

class Server
{
public:
	Server();
	~Server();

	void Initialize(std::uint32_t port);

	Network GameNetwork;

	
private:

};
