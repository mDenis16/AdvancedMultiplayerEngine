#include "Streamer.hpp"

#include <Common.hpp>
#include <Shared.hpp>
#include <enet\enet.h>

#include "Network.h"

#include <functional>
#include <set>

#include "PacketHandler.hpp"	

void Streamer::Init(int threads, int distance)
{
	glm::vec3 gtaMins = glm::vec3(-5395.54, -5024.13, 0);
	glm::vec3 gtaMaxs = glm::vec3(5395.54, 9024.13, 0);
	grid = new Grid<Entity*>(gtaMins, gtaMaxs, glm::vec3(distance / 3, distance / 3, 0));

	THREAD_START(HandleEntityStream, std::bind(&Streamer::HandleEntityStream, this));
}
void Streamer::UpdateStream(Player* player)
{
	if (GameNetwork.streamer.grid == nullptr) return;

	std::cout << "Streamer : Update stream for player " << std::endl;

	SAFE_MODIFY(EntityStreamQueue);
	EntityStreamQueue.push(player);
	THREAD_WAKE(HandleEntityStream);
}

void Streamer::HandleEntityStream()
{
	debug("Streamer : Create => HandleEntityStream");
	while (true)
	{
		THREAD_WAIT(HandleEntityStream)
			SAFE_MODIFY(EntityStreamQueue);
			while (EntityStreamQueue.size() > 0) {
				Player* player = EntityStreamQueue.front();
				EntityStreamQueue.pop();

				
				}
			}

		THREAD_SLEEP(HandleEntityStream);
	}
}
