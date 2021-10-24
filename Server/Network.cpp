#include <Common.hpp>
#include <Shared.hpp>
#include <enet\enet.h>

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

	
	Player* newPlayer = new Player(peer, GenerateHandle());
	peer->data = newPlayer;

	this->Entities[newPlayer->EntityHandle] = newPlayer;
	SAFE_READ(Players);
	Players.push_back(newPlayer);

	auto packet = new NetworkPacket(PacketType::PlayerJoin, nullptr, true, 0, peer);

	packet->Write(newPlayer->EntityHandle);

	packet->Send();


	
	
}
/// <summary>
/// Handles outgoing flow
/// </summary>
///
///
///
///
void Network::HandleOutgoingFlow()
{
	debug("NetworkThread : Create => HandleOutgoingFlow");
	while (true) {

		THREAD_WAIT(HandleOutgoingFlow)
		SAFE_MODIFY(OutgoingPackets);

		while (OutgoingPackets.size() > 0) {
			NetworkPacket* message = OutgoingPackets.front();
			OutgoingPackets.pop();


			ENetPacket* newPacket = enet_packet_create(message->mData, message->Lenght, ENET_PACKET_FLAG_RELIABLE | ENET_PACKET_FLAG_NO_ALLOCATE);
			newPacket->userData = message;
			newPacket->freeCallback = OnNetworkReleaseMessage;

			if (message->PacketFlags & PacketFlags::StreamEntireNetwork)
			{
				SAFE_READ(Players);
				for (auto player : Players)
					if (player->Peer != message->ExceptPeer) {
						enet_peer_send(player->Peer, 0, newPacket);
					}
			}/*else if (message->PacketFlags & StreamRangeNetwork)
			{
				auto player = (Player*)message->TargetPeer->data;
				if (player) {
					
					std::lock_guard<std::mutex> guard(player->EntitiesInStreamRangeMutex);
					for (auto streamed : player->EntitiesInStreamRange) {
						if (streamed->Type == EntityType::ET_Player)
						enet_peer_send(streamed->Peer, 0, newPacket);
					}
						
				}
			}*/
			else if (message->PacketFlags & StreamRangeEntities)
			{
				
				for (auto batch : message->EntitiesBatch) {
					if (batch->Type == EntityType::ET_Player)
						enet_peer_send(batch->Peer, 0, newPacket);
				}
			}
			else
			{
				enet_peer_send(message->TargetPeer, 0, newPacket);
			}
			 
			std::cout << "Sent packet type " << message->Type << " with size of " << sizeof(message->mData) << std::endl;
		

		}

		THREAD_SLEEP(HandleOutgoingFlow);
	}
}
void Network::HandleFlow() {

	debug("\nNetworkThread : Create => HandleFlow");
	while (true) {

		

		ENetEvent ev;
		bool NeedWake = false;
		while (enet_host_service(HostListener, &ev, 0) > 0)
			if (ev.type == ENET_EVENT_TYPE_CONNECT) {
				OnClientConnect(ev.peer);
			}
			else if (ev.type == ENET_EVENT_TYPE_DISCONNECT)
			{
			
			}
			else if (ev.type == ENET_EVENT_TYPE_RECEIVE) {
				SAFE_MODIFY(IncomingPackets);

				NetworkPacket* receivedPacket = new NetworkPacket(ev.peer, ev.packet);

				IncomingPackets.push(receivedPacket);
				NeedWake = true;
			}

		if (NeedWake)
			THREAD_WAKE(HandleIncomingFlow);

		Sleep(1000 / 40);

	}

}
void Network::Initialize(std::uint32_t port, int maxClients)
{

	enet_initialize();
	ENetAddress addr;

	enet_address_set_host(&addr, "192.168.88.252");

	addr.port = port;
	HostListener = enet_host_create(&addr, maxClients, 1, 0, 0);

	if (!HostListener) {
		std::cout << "Failed initalize listener." << std::endl;
	}

	std::cout << "Listening on port " << port << std::endl;

	streamer.Init(1, 500);

	THREAD_START(HandleIncomingFlow, std::bind(&Network::HandleIncomingFlow, this));
	THREAD_START(HandleOutgoingFlow, std::bind(&Network::HandleOutgoingFlow, this));
	THREAD_START(HandleFlow, std::bind(&Network::HandleFlow, this));


}

void Network::HandleIncomingFlow()
{
	debug("NetworkThread : Create => HandleIncomingFlow");

	while (true) {

		THREAD_WAIT(HandleIncomingFlow);
		
		SAFE_MODIFY(IncomingPackets);

		while (IncomingPackets.size()) {
			NetworkPacket* message = IncomingPackets.front();
	

			message->MarkAsExecuted();

			Player* targetPlayer = (Player*)message->TargetPeer->data;


			if (targetPlayer)
		    	PacketHandler::Process(targetPlayer, message);


			IncomingPackets.pop();
			if (message->Executed) {
				delete message;
			}

		}


		THREAD_SLEEP(HandleIncomingFlow);
	}
}