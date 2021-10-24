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

	//	std::cout << "Streamer : Update stream for player " << std::endl;

	SAFE_MODIFY(EntityStreamQueue);
	EntityStreamQueue.push(player);
	THREAD_WAKE(HandleEntityStream);
}

/// <summary>
/// This handles stream for player who requested at chaning cell
/// </summary>
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


			auto cell = &grid->cells[player->cellIndex];
			auto oldCell = &grid->cells[player->oldCellIndex];

			if (cell && oldCell) {

				std::vector<Entity*> streamIn;
				std::vector<Entity*> streamOut;

				for (auto& nearbyCell : grid->getNearby(cell))
					for (auto& entity : nearbyCell->Items)
					{
						if (entity->EntityHandle == player->EntityHandle) continue;


						auto itx = std::find(player->EntitiesInStreamRange.begin(), player->EntitiesInStreamRange.end(), entity);
						if (itx == player->EntitiesInStreamRange.end()) {
							std::cout << "ENTITY " << entity->EntityHandle << " is NOW STREAMED for " << player->EntityHandle << std::endl;
							std::cout << "ENTITY " << player->EntityHandle << " is NOW STREAMED for " << entity->EntityHandle << std::endl;
							entity->EntitiesInStreamRange.push_back(player);
							player->EntitiesInStreamRange.push_back(entity);
							if (entity->EntityHandle != player->EntityHandle)
							streamIn.push_back(entity);
						}
					}


				for (int i = 0; i < player->EntitiesInStreamRange.size(); i++) {


					auto entity = player->EntitiesInStreamRange.at(i);
					if (entity->EntityHandle == player->EntityHandle) continue;

					auto entCell = &grid->cells[entity->cellIndex];

					if (!grid->isNearby(cell, entCell))
					{

						std::cout << "ENTITY " << entity->EntityHandle << " is NO LONGER STREAMED for " << player->EntityHandle << std::endl;
						std::cout << "ENTITY " << player->EntityHandle << " is NO LONGER STREAMED for " << entity->EntityHandle << std::endl;
						player->EntitiesInStreamRange.erase(player->EntitiesInStreamRange.begin() + i);

						auto itx = std::find(entity->EntitiesInStreamRange.begin(), entity->EntitiesInStreamRange.end(), player);
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
					packetToStreamed->Write(player->EntityHandle);
					packetToStreamed->Write(player->Type);
					packetToStreamed->Write(player->Position);

					packetToStreamed->Send();

					/// <summary>
				/// Tell player streamed entities
				/// </summary>
					auto packetToPlayer = new NetworkPacket(EntitiesStreamIn, player->Peer, ENET_PACKET_FLAG_RELIABLE);
					packetToPlayer->Write((std::uint32_t)streamIn.size());
					for(auto& ent : streamIn )
					{
						packetToPlayer->Write(ent->EntityHandle);
						packetToPlayer->Write(ent->Type);
						packetToPlayer->Write(ent->Position);
					}

					packetToPlayer->Send();
				}
				if (!streamOut.empty())
				{

					/// <summary>
					/// Tell stream entities player joined their stream range
					/// </summary>
					auto packetToStreamed = new NetworkPacket(EntitiesStreamOut, streamOut, StreamRangeEntities);
					packetToStreamed->Write((std::uint32_t)1);
					packetToStreamed->Write(player->EntityHandle);
					packetToStreamed->Write(player->Type);
					packetToStreamed->Write(player->Position);

					packetToStreamed->Send();

					/// <summary>
				/// Tell player streamed entities
				/// </summary>
					auto packetToPlayer = new NetworkPacket(EntitiesStreamOut, player->Peer, true);
					packetToPlayer->Write((std::uint32_t)streamOut.size());
					for (auto& ent : streamOut)
					{
						packetToPlayer->Write(ent->EntityHandle);
						packetToPlayer->Write(ent->Type);
						packetToPlayer->Write(ent->Position);
					}

					packetToPlayer->Send();
				}
				if (!streamIn.empty() || !streamOut.empty()) {
					std::cout << "Handled stream for player " << player->EntityHandle << std::endl;

					std::cout << "Results : " << std::endl;

					std::cout << "New streamed in entities count " << streamIn.size() << std::endl;
					for (auto& ent : streamIn)
						std::cout << "EntityHandle " << ent->EntityHandle << std::endl;
					std::cout << "New streamed out entities count " << streamOut.size() << std::endl;
					for (auto& ent : streamOut)
						std::cout << "EntityHandle " << ent->EntityHandle << std::endl;

				}
			}
		}

		THREAD_SLEEP(HandleEntityStream);
	}
}
