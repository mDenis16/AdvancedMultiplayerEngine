#pragma once


class Network
{
public:
	Network(std::uint32_t port, int maxClients);
	~Network();

	std::unordered_map <std::uint32_t, Entity*> EntityList;


	void Initialize(std::uint32_t port, int maxClients);

private:

	ENetHost* HostListener = nullptr;
};

