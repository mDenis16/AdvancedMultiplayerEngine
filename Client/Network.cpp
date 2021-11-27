
#include <Shared.hpp>

#include <functional>

#include "Network.hpp"

void Network::Initialize()
{	
	std::cout << "Network construct called " << std::endl;

	if (enet_initialize() < 0) {
		std::cout << "Failed to initialize ENet!" << std::endl;
		return;
	}
	std::cout << "ENet initialized." << std::endl;

	Local = enet_host_create(nullptr, 1, 1, 0, 0);

	Running = true;

//	THREAD_START(HandleIncomingFlow, std::bind(&Network::HandleIncomingFlow, this));
	//THREAD_START(HandleOutgoingFlow, std::bind(&Network::HandleOutgoingFlow, this));
	//THREAD_START(HandleFlow, std::bind(&Network::HandleFlow, this));
	THREAD_START(ProcessNetwork, std::bind(&Network::ProcessNetwork, this));


	THREAD_START(NetworkThread, std::bind(&Network::NetworkThread, this));
	int distance = 300;

	glm::vec3 gtaMins = glm::vec3(-5395.54, -5024.13, 0);
	glm::vec3 gtaMaxs = glm::vec3(5395.54, 9024.13, 0);
	grid = new Grid<Entity*>(gtaMins, gtaMaxs, glm::vec3(distance / 3, distance / 3, 0));


}
void Network::Destroy()
{
	
	Running = false;

	std::lock_guard<std::mutex> guard(m_action_lock);
	m_action_cond.notify_one();

	
	if (Peer)
		Disconnect();
	Connected = false;
	




	if (Local) {
		enet_host_destroy(Local);
		enet_deinitialize();

		Local = nullptr;
	}
}

void Network::NetworkThread() {

	while (Running) {
	
		ENetEvent ev;
		bool NeedWake = false;
		while (enet_host_service(Local, &ev, 1) > 0)
			if (ev.type == ENET_EVENT_TYPE_CONNECT) {
				std::cout << "Conncted succesfully to server." << std::endl;
				this->SendHandshake();
				this->EventConnect();
				Connected = true;
			}
			else if (ev.type == ENET_EVENT_TYPE_DISCONNECT)
			{
				Disconnect();
				std::cout << "Disconnected " << std::endl;
			}
			else if (ev.type == ENET_EVENT_TYPE_RECEIVE) {
				//std::cout << "Client received packet" << std::endl;
 				/*SAFE_MODIFY(IncomingPackets);

				NetworkPacket* receivedPacket = new NetworkPacket(ev.peer, ev.packet);

				IncomingPackets.push(receivedPacket);
				NeedWake = true;*/


				auto receivedPacket = new NetworkPacket(ev.peer, ev.packet);
				{
					std::lock_guard<std::mutex> guard(m_action_lock);
					//	std::cout << "Received packet type " << receivedPacket->Type << std::endl;
					m_actions.push(receivedPacket);
				}

				m_action_cond.notify_one();

			}

		
	
	}

	std::cout << "Network thread stopped!!! " << std::endl;
	
}


void Network::ProcessNetwork()
{
	/*_CrtSetDbgFlag(
		_CRTDBG_ALLOC_MEM_DF |
		_CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR,

		_CRTDBG_MODE_DEBUG);*/
	while (Running) {
		std::unique_lock<std::mutex> lock(m_action_lock);

		while (m_actions.empty()) {
	//		std::cout << "m_actions empty " << std::endl;
			m_action_cond.wait(lock);
		}
	//	std::cout << "Process network " << std::endl;

		auto message = m_actions.front();
		m_actions.pop();

		lock.unlock();
		if (message->Type == PlayerJoin) {

			//	std::cout << "Player joined" << std::endl;

			std::uint32_t Handle;

			message->Read(Handle);

			//	Entities[Handle] = new Player(Handle);

		}
		else if (message->Type == PlayerCreateMove)
		{
			std::uint32_t Handle;

			message->Read(Handle);


			{
				std::lock_guard<std::mutex> guard(streamLock);

				auto it = std::find_if(StreamedEntities.begin(), StreamedEntities.end(), [Handle](Entity* s) {
					return s->EntityHandle == Handle;
					});
				if (it != StreamedEntities.end()) {
//					std::cout << "Received CreateMove from handle " << Handle << std::endl;

					(*it)->CreateMove(message);


				}
			}
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

	}

	std::cout << "Process network stopped!!! " << std::endl;

}
//void Network::HandleIncomingFlow()
//{
//
//	while (Running) {
//		
//		THREAD_WAIT(HandleIncomingFlow)
//		SAFE_MODIFY(IncomingPackets)
//
//		while (IncomingPackets.size() > 0) {
//			NetworkPacket* message = IncomingPackets.front();
//			IncomingPackets.pop();
//
//			message->MarkAsExecuted();
//
//			//std::cout << "Handle incmming packet type " << message->Type << std::endl;
//			
//			
//
//			if (message->Executed) {
//				delete message;
//			}
//		}
//
//
//		THREAD_SLEEP(HandleIncomingFlow);
//	}
//	std::cout << "HandleIncomingFlow stopped " << std::endl;
//}
//
//
//void Network::HandleOutgoingFlow()
//{
//	while (Running) {
//		
//
//		THREAD_WAIT(HandleOutgoingFlow)
//		SAFE_MODIFY(OutgoingPackets);
//
//		while (OutgoingPackets.size() > 0) {
//			NetworkPacket* message = OutgoingPackets.front();
//			OutgoingPackets.pop();
//
//		
//			ENetPacket* newPacket = enet_packet_create(message->mData, message->Lenght, ENET_PACKET_FLAG_RELIABLE | ENET_PACKET_FLAG_NO_ALLOCATE);
//			newPacket->userData = message;
//			newPacket->freeCallback = OnNetworkReleaseMessage;
//			enet_peer_send(Peer, 0, newPacket);
//
//		}
//
//		enet_host_flush(Local);
//		THREAD_SLEEP(HandleOutgoingFlow);
//	}
//	
//
//	std::cout << "HandleOutgoingFlow stopped " << std::endl;
//}
void Network::SendHandshake()
{
	auto packet = new NetworkPacket(PlayerHandshake, ENET_PACKET_FLAG_RELIABLE);

	//std::cout << "Added packet handshake to queue " << std::endl;
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
	if (Peer) {
		std::cout << "Connected to host " << std::endl;
	}
}
void Network::EntityCM(Entity* ent)
{
	
}
void Network::Disconnect()
{
	if (!Peer) return;


	std::cout << "Disconnnecting from " << Peer->address.host << std::endl;

	enet_peer_disconnect(Peer, 0);
	Connected = false;
	Running = false;
	Peer = nullptr;
}

void Network::NetworkCreateLagRecord(LagRecord& cmd)
{

	auto movePacket = new NetworkPacket(PlayerCreateMove, 0);

	movePacket->Write< LagRecord>(cmd);


	movePacket->Send();
}
