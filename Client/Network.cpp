
#include <Shared.hpp>

#include <functional>

#include "Network.hpp"

Network::Network()
{	
	THREAD_START(HandleIncomingFlow, std::bind(&Network::HandleIncomingFlow, this));
	THREAD_START(HandleOutgoingFlow, std::bind(&Network::HandleOutgoingFlow, this));

}

Network::~Network()
{
}

void Network::HandleFlow() {
	while (true) {

		SAFE_MODIFY(IncomingPackets);

		while (IncomingPackets.size() > 0) {
			NetworkPacket* message = IncomingPackets.front();
		
			message->MarkAsExecuted();

			//HandleMessage(message);

			if (message->Executed) {
				delete message;
			}
		}

		Sleep(1000 / 30);
	}
}
void Network::HandleIncomingFlow()
{
	while (true) {
		ENetEvent ev;
		while (enet_host_service(Local, &ev, 0) > 0)
			if (ev.type == ENET_EVENT_TYPE_CONNECT) {
				this->SendHandshake();
				this->EventConnect();
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
			}
	}
	Sleep(1000 / 30);
}


void Network::HandleOutgoingFlow()
{
	while (true) {
		THREAD_WAIT(HandleOutgoingFlow)
		while (OutgoingPackets.size() > 0) {
			NetworkPacket* message = OutgoingPackets.front();
			OutgoingPackets.pop();

		
			ENetPacket* newPacket = enet_packet_create(message->mData, message->Lenght, ENET_PACKET_FLAG_RELIABLE | ENET_PACKET_FLAG_NO_ALLOCATE);
			newPacket->userData = message;
			newPacket->freeCallback = OnNetworkReleaseMessage;
			enet_peer_send(Peer, 0, newPacket);

		}
	}
}
void Network::SendHandshake()
{
}
void Network::Connect(std::string& hostname, std::uint16_t port)
{
	ENetAddress addr;
	enet_address_set_host(&addr, hostname.c_str());
	addr.port = port;
	Peer = enet_host_connect(Local, &addr, 1, 0);

}
