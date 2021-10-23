
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

	Connected = true;

	THREAD_START(HandleIncomingFlow, std::bind(&Network::HandleIncomingFlow, this));
	THREAD_START(HandleOutgoingFlow, std::bind(&Network::HandleOutgoingFlow, this));
	THREAD_START(HandleFlow, std::bind(&Network::HandleFlow, this));

	int distance = 500;

	glm::vec3 gtaMins = glm::vec3(-5395.54, -5024.13, 0);
	glm::vec3 gtaMaxs = glm::vec3(5395.54, 9024.13, 0);
	grid = new Grid<Entity*>(gtaMins, gtaMaxs, glm::vec3(distance / 3, distance / 3, 0));

}
Network::~Network()
{
	Connected = false;

	if (Peer)
		Disconnect();


	if (Local) {
		enet_host_destroy(Local);
		enet_deinitialize();

		Local = nullptr;
	}
}

void Network::HandleFlow() {

	while (Connected) {
		
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
				std::cout << "Disconnected " << std::endl;
				Peer = nullptr;
				Connected = false;
			}
			else if (ev.type == ENET_EVENT_TYPE_RECEIVE) {
				std::cout << "Client received packet" << std::endl;
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

	while (Connected) {
		
		THREAD_WAIT(HandleIncomingFlow)
		SAFE_MODIFY(IncomingPackets)

		while (IncomingPackets.size() > 0) {
			NetworkPacket* message = IncomingPackets.front();
			IncomingPackets.pop();

			message->MarkAsExecuted();

			//std::cout << "Handle incmming packet type " << message->Type << std::endl;

			if (message->Type == PlayerJoin) {

				std::cout << "Player joined" << std::endl;

				std::uint32_t Handle;

				message->Read(Handle);

				Entities[Handle] = new Player(Handle);

			}
			else if (message->Type == EntitiesStreamIn)
			{
				
				ProcessStream(message);
			}
			else if (message->Type == EntitiesStreamOut)
			{

				ProcessStreamOut(message);
			}
			else
			    HandlePacket(message);

			if (message->Executed) {
				delete message;
			}
		}


		THREAD_SLEEP(HandleIncomingFlow);
	}
	
}


void Network::HandleOutgoingFlow()
{
	while (Connected) {
		
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
	if (Local == nullptr || Peer != nullptr) {
		return;
	}

	ENetAddress addr;
	enet_address_set_host(&addr, hostname.c_str());
	addr.port = port;
	Peer = enet_host_connect(Local, &addr, 1, 0);

	Connected = true;
}

void Network::Disconnect()
{
	if (!Peer) return;


	std::cout << "Disconnnecting from " << Peer->address.host << std::endl;

	enet_peer_disconnect(Peer, 0);
	Peer = nullptr;
}

void Network::NetworkCreateMove(ClientCmd& cmd)
{

	auto movePacket = new NetworkPacket(PlayerCreateMove, 0);

	movePacket->Write(cmd.Position);
	movePacket->Write(cmd.Rotation);
	movePacket->Write(cmd.Direction);
	movePacket->Write(cmd.cellIndex);

	movePacket->Send();
}
