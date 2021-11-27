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
void Streamer::UpdateStream(Entity* player)
{
	if (GameNetwork.streamer.grid == nullptr) return;

	//	std::cout << "Streamer : Update stream for player " << std::endl;

	{
		std::lock_guard<std::mutex> guard(streamRequests_lock);
		streamRequests.push(player);
		
	}
	HandleEntityStreamMutexCond.notify_one();
}

/// <summary>
/// This handles stream for player who requested at chaning cell
/// </summary>
void Streamer::HandleEntityStream()
{
	debug("Streamer : Create => HandleEntityStream");
	while (true)
	{

		std::unique_lock<std::mutex> lock(streamRequests_lock);

		while (streamRequests.empty()) {
			HandleEntityStreamMutexCond.wait(lock);
		}

		Entity* requestedSteamer = streamRequests.front();
		streamRequests.pop();

		lock.unlock();

		std::lock_guard<std::mutex> lockStreamRange(requestedSteamer->EntitiesInStreamRangeMutex);

		SAFE_MODIFY(grid);

		auto cell = &grid->cells[requestedSteamer->cellIndex];
		auto oldCell = &grid->cells[requestedSteamer->oldCellIndex];

		if (cell && oldCell) {
			
			

			auto it = std::find(oldCell->Items.begin(), oldCell->Items.end(), requestedSteamer);
			if (it != oldCell->Items.end()) {
				std::cout << "Removed entity from old cell." << std::endl;
				oldCell->Items.erase(it);
			}

			cell->Items.push_back(requestedSteamer);

			std::vector<Entity*> streamIn; streamIn.reserve(250);
			std::vector<Entity*> streamOut; streamOut.reserve(250);

			for (auto& nearbyCell : grid->getNearby(cell))
				for (auto& entity : nearbyCell->Items)
				{
					if (entity->EntityHandle == requestedSteamer->EntityHandle) continue;


					auto itx = std::find(requestedSteamer->EntitiesInStreamRange.begin(), requestedSteamer->EntitiesInStreamRange.end(), entity);
					if (itx == requestedSteamer->EntitiesInStreamRange.end()) {
						std::cout << "ENTITY " << entity->EntityHandle << " is NOW STREAMED for " << requestedSteamer->EntityHandle << std::endl;
						std::cout << "ENTITY " << requestedSteamer->EntityHandle << " is NOW STREAMED for " << entity->EntityHandle << std::endl;


						entity->EntitiesInStreamRange.push_back(requestedSteamer);
						requestedSteamer->EntitiesInStreamRange.push_back(entity);

						streamIn.push_back(entity);

					}
				}


			for (int i = 0; i < requestedSteamer->EntitiesInStreamRange.size(); i++) {


				auto entity = requestedSteamer->EntitiesInStreamRange.at(i);
				if (entity->EntityHandle == requestedSteamer->EntityHandle)
				{
					requestedSteamer->EntitiesInStreamRange.erase(requestedSteamer->EntitiesInStreamRange.begin() + i);
					std::cout << "BUG DELETED STREMED PLAYER TO OWN!!!" << std::endl;
					continue;
				};

				auto entCell = &grid->cells[entity->cellIndex];

				if (!grid->isNearby(cell, entCell))
				{

					std::cout << "ENTITY " << entity->EntityHandle << " is NO LONGER STREAMED for " << requestedSteamer->EntityHandle << std::endl;
					std::cout << "ENTITY " << requestedSteamer->EntityHandle << " is NO LONGER STREAMED for " << entity->EntityHandle << std::endl;
					requestedSteamer->EntitiesInStreamRange.erase(requestedSteamer->EntitiesInStreamRange.begin() + i);

					auto itx = std::find(entity->EntitiesInStreamRange.begin(), entity->EntitiesInStreamRange.end(), requestedSteamer);
					if (itx != entity->EntitiesInStreamRange.end()) {
						entity->EntitiesInStreamRange.erase(itx);
					}

					streamOut.push_back(entity);

				}

			}

			if (!streamIn.empty())
			{


				/// <summary>
				/// Tell stream entities player joined their stream range
				/// </summary>
				auto packetToStreamed = new NetworkPacket(EntitiesStreamIn, streamIn, StreamRangeEntities);
				packetToStreamed->Write((std::uint32_t)1);
				packetToStreamed->Write(requestedSteamer->EntityHandle);
				packetToStreamed->Write(requestedSteamer->Type);
				packetToStreamed->Write(requestedSteamer->Position);

				packetToStreamed->Send();

				if (requestedSteamer->Type == EntityType::ET_Player) {
					/// <summary>
				    /// Tell player streamed entities
					/// </summary>
					auto packetToPlayer = new NetworkPacket(EntitiesStreamIn, requestedSteamer->Peer, ENET_PACKET_FLAG_RELIABLE);
					packetToPlayer->Write((std::uint32_t)streamIn.size());
					for (auto& ent : streamIn)
					{
						packetToPlayer->Write(ent->EntityHandle);
						packetToPlayer->Write(ent->Type);
						packetToPlayer->Write(ent->Position);
					}
					packetToPlayer->Send();
				}


			}
			if (!streamOut.empty())
			{

				/// <summary>
				/// Tell stream entities player joined their stream range
				/// </summary>
				auto packetToStreamed = new NetworkPacket(EntitiesStreamOut, streamOut, StreamRangeEntities);
				packetToStreamed->Write((std::uint32_t)1);
				packetToStreamed->Write(requestedSteamer->EntityHandle);

				packetToStreamed->Send();

				if (requestedSteamer->Type == EntityType::ET_Player) {
					/// Tell player streamed entities
					/// </summary>
					auto packetToPlayer = new NetworkPacket(EntitiesStreamOut, requestedSteamer->Peer, true);
					packetToPlayer->Write((std::uint32_t)streamOut.size());
					for (auto& ent : streamOut)
						packetToPlayer->Write(ent->EntityHandle);


					packetToPlayer->Send();
				}

			}
		/*	if (!streamIn.empty() || !streamOut.empty()) {
				std::cout << "Handled stream for player " << requestedSteamer->EntityHandle << std::endl;

				std::cout << "Results : " << std::endl;

				std::cout << "New streamed in entities count " << streamIn.size() << std::endl;
				for (auto& ent : streamIn)
					std::cout << "EntityHandle " << ent->EntityHandle << std::endl;
				std::cout << "New streamed out entities count " << streamOut.size() << std::endl;
				for (auto& ent : streamOut)
					std::cout << "EntityHandle " << ent->EntityHandle << std::endl;

			}*/
		}
	}
}