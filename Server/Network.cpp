#include <Common.hpp>
#include <Shared.hpp>
#include <enet/enet.h>

#include "Network.h"

#include <functional>
#include "PacketHandler.hpp"

/// <summary>
/// Tell everyone on the server the player has joined
///	in order to add in entity map and player vector
/// </summary>
/// <param name="peer"></param>
void Network::OnClientConnect(ENetPeer* peer)
{
	printf("New connection from: %08x:%d \n", peer->address.host, peer->address.port);


	auto newPlayer = new Player(peer, GenerateHandle());
	peer->data = newPlayer;
	//newPlayer->EntitiesInStreamRange.reserve(5000);

	SAFE_READ(Entities);
	Entities[newPlayer->EntityHandle] = newPlayer;

	for (int i = 0; i < 1; ++i)
	{
		auto newPed = new Ped(GenerateHandle());
		newPed->Controller = newPlayer;

		newPlayer->controlledPeds.push_back(newPed);
		Entities[newPed->EntityHandle] = newPed;
		newPed->cellIndex = 0;
		newPed->oldCellIndex = 0;

	}

	
}

void Network::OnClientDisconnect(ENetPeer* peer)
{
	printf("Client disconnected from: %08x:%d \n", peer->address.host, peer->address.port);

	auto player = static_cast<Player*>(peer->data);


	{
		SAFE_READ(streamer.grid);

		auto cell = &streamer.grid->cells[player->cellIndex];
		if (cell)
		{
			auto itx = std::find(cell->Items.begin(), cell->Items.end(), player);
			if (itx != cell->Items.end())
				cell->Items.erase(itx);
		}
	}
	{
		SAFE_READ(Entities);
		Entities[player->EntityHandle] = nullptr;
	}
	{
		std::lock_guard<std::mutex> lock(player->EntitiesInStreamRangeMutex);
		for (auto& ent : player->EntitiesInStreamRange)
		{
			auto itx = std::find(ent->EntitiesInStreamRange.begin(), ent->EntitiesInStreamRange.end(), player);
			if (itx != ent->EntitiesInStreamRange.end())
			{
				ent->EntitiesInStreamRange.erase(itx);
			}
		}

		auto packetToStreamed = new NetworkPacket(EntitiesStreamOut, player->EntitiesInStreamRange, StreamRangeEntities);
		packetToStreamed->Write(static_cast<std::uint32_t>(1));
		packetToStreamed->Write(player->EntityHandle);

		packetToStreamed->Send();
	}

	delete player;
}
void Network::Initialize(std::uint32_t port, int maxClients)
{
	enet_initialize();
	ENetAddress addr;

	enet_address_set_host(&addr, "192.168.88.252");

	addr.port = port;
	HostListener = enet_host_create(&addr, maxClients, 1, 0, 0);

	if (!HostListener)
	{
		std::cout << "Failed initalize listener." << std::endl;
	}

	std::cout << "Listening on port " << port << std::endl;

	streamer.Init(1, 300);


	THREAD_START(ProcessNetwork, std::bind(&Network::ProcessNetwork, this));


	THREAD_START(NetworkThread, std::bind(&Network::NetworkThread, this));
	//THREAD_START(HandleIncomingFlow, std::bind(&Network::HandleIncomingFlow, this));
	//THREAD_START(HandleOutgoingFlow, std::bind(&Network::HandleOutgoingFlow, this));

}

/// <summary>
/// Handles outgoing flow
/// </summary>
///
///
///
///

void Network::ProcessNetwork()
{
	/*_CrtSetDbgFlag(
		_CRTDBG_ALLOC_MEM_DF |
		_CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR,

		_CRTDBG_MODE_DEBUG);*/
	while (1) {
		

		std::unique_lock<std::mutex> lock(m_action_lock);

		Network::CurrentFrameTime = enet_time_get();

		while (m_actions.empty()) {
			std::cout << "m_actions empty " << std::endl;
			m_action_cond.wait(lock);
		}
		std::cout << "Process network " << std::endl;

		auto a = m_actions.front();
		m_actions.pop();

		lock.unlock();
		auto targetPlayer = static_cast<Player*>(a->TargetPeer->data);
		PacketHandler::Process(targetPlayer, a);

	}
}

void Network::NetworkThread()
{
	/*_CrtSetDbgFlag(
		_CRTDBG_ALLOC_MEM_DF |
		_CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR,
		_CRTDBG_MODE_DEBUG);*/
	debug("\nNetworkThread : Create => HandleFlow");
	while (true)
	{
	//	debug("\nNetworkThread : Run => HandleFlow");
		//SAFE_READ(HostListener);

		ENetEvent ev;


		bool needUpdate = true;

		while (enet_host_service(HostListener, &ev, 1) > 0) {
			if (ev.type == ENET_EVENT_TYPE_CONNECT)
			{
				OnClientConnect(ev.peer);
			}
			else if (ev.type == ENET_EVENT_TYPE_DISCONNECT)
			{
				OnClientDisconnect(ev.peer);
			}
			else if (ev.type == ENET_EVENT_TYPE_RECEIVE)
			{
				//debug("\nReceived event ");

				auto receivedPacket = new NetworkPacket(ev.peer, ev.packet);
				{
					std::lock_guard<std::mutex> guard(m_action_lock);
				//	std::cout << "Received packet type " << receivedPacket->Type << std::endl;
					m_actions.push(receivedPacket);
				}
				
				m_action_cond.notify_one();

			}

			
		}
	
		
	}
}

