#include <Common.hpp>
#include <Shared.hpp>
#include <enet\enet.h>

#include "Network.h"

void Network::Initialize(std::uint32_t port, int maxClients)
{
	ENetAddress addr;
	enet_address_set_host(&addr, "127.0.0.1");
	addr.port = port;
	HostListener = enet_host_create(&addr, maxClients, 1, 0, 0);


	while (true) {
		bool WakeupExecutor = false;

		ENetEvent ev;
		while (enet_host_service(HostListener, &ev, 0) > 0) {
			if (ev.type == ENET_EVENT_TYPE_CONNECT) {
				//OnClientConnect(ev.peer);
			}
			else if (ev.type == ENET_EVENT_TYPE_DISCONNECT) {
				//OnClientDisconnect(ev.peer);
			}
			else if (ev.type == ENET_EVENT_TYPE_RECEIVE) {
				NetworkPacket* newMessage = new NetworkPacket(ev.peer, ev.packet);

				SAFE_MODIFY(m_incomingMessages);
				m_incomingMessages.push(newMessage);
				
				WakeupExecutor = true;
			}
		}

		if (WakeupExecutor)
			THREAD_WAKE(Executor);
		
	}

}
void Network::QueueExecutor()
{
	debug("NetworkThread : Create => QueueExecutor");

	while (true) {

		THREAD_WAIT(Executor);
		
		SAFE_MODIFY(IncomingPackets);

		while (IncomingPackets.size()) {
			NetworkPacket* message = IncomingPackets.front();
	

			message->MarkAsExecuted();

			Player* targetPlayer = (Player*)message->TargetPeer->data;

			//targetPlayer->HandleMessage(message);

			IncomingPackets.pop();
			if (message->Executed) {
				delete message;
			}

		}

		THREAD_SLEEP(Executor);
	}
}