
#include <Shared.hpp>

#include <functional>

#include "Network.hpp"

Network::Network()
{	
	std::cout << "Network construct called " << std::endl;

	if (enet_initialize() < 0) {
		std::cout << "Failed to initialize ENet!" << std::endl;
		return;
	}
	std::cout << "ENet initialized." << std::endl;

	Local = enet_host_create(nullptr, 1, 1, 0, 0);

	THREAD_START(HandleIncomingFlow, std::bind(&Network::HandleIncomingFlow, this));
	THREAD_START(HandleOutgoingFlow, std::bind(&Network::HandleOutgoingFlow, this));
	THREAD_START(HandleFlow, std::bind(&Network::HandleFlow, this));
}

Network::~Network()
{
}

void Network::HandleFlow() {

	while (true) {
		
		ENetEvent ev;
		bool NeedWake = false;
		while (enet_host_service(Local, &ev, 0) > 0)
			if (ev.type == ENET_EVENT_TYPE_CONNECT) {
				std::cout << "Conncted succesfully to server." << std::endl;
				this->SendHandshake();
				this->EventConnect();
				Connected = true;
			}
			else if (ev.type == ENET_EVENT_TYPE_DISCONNECT)
			{
				Peer = nullptr;
				Connected = false;
			}
			else if (ev.type == ENET_EVENT_TYPE_RECEIVE) {
				SAFE_MODIFY(IncomingPackets);

				NetworkPacket* receivedPacket = new NetworkPacket(ev.peer, ev.packet);

				IncomingPackets.push(receivedPacket);
				NeedWake = true;
			}

		if (NeedWake)
			THREAD_WAKE(HandleIncomingFlow);

	}
	
}
void Network::HandleIncomingFlow()
{

	while (true) {
		
		THREAD_WAIT(HandleOutgoingFlow)
		SAFE_MODIFY(IncomingPackets);

		while (IncomingPackets.size() > 0) {
			NetworkPacket* message = IncomingPackets.front();

			message->MarkAsExecuted();

			HandlePacket(message);

			if (message->Executed) {
				delete message;
			}
		}

		{
			SAFE_MODIFY(OutgoingPackets);
			if (OutgoingPackets.size() > 0)
				THREAD_WAKE(HandleOutgoingFlow);
		}
		THREAD_SLEEP(HandleIncomingFlow);
	}
	
}


void Network::HandleOutgoingFlow()
{
	while (true) {
		
		THREAD_WAIT(HandleOutgoingFlow)
		SAFE_MODIFY(OutgoingPackets);

		while (OutgoingPackets.size() > 0) {
			NetworkPacket* message = OutgoingPackets.front();
			OutgoingPackets.pop();

		
			ENetPacket* newPacket = enet_packet_create(message->mData, message->Lenght, message->Flags);
			newPacket->userData = message;
			newPacket->freeCallback = OnNetworkReleaseMessage;
			enet_peer_send(Peer, 0, newPacket);

		}

		THREAD_SLEEP(HandleOutgoingFlow);
	}
}
void Network::SendHandshake()
{
	auto packet = new NetworkPacket(PlayerHandshake, ENET_PACKET_FLAG_RELIABLE);

	std::cout << "Added packet handshake to queue " << std::endl;
	packet->Send();
}
void Network::Connect(std::string hostname, std::uint16_t port)
{
	ENetAddress addr;
	enet_address_set_host(&addr, hostname.c_str());
	addr.port = port;
	Peer = enet_host_connect(Local, &addr, 1, 0);

	Connected = true;
}
