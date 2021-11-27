
#include <Shared.hpp>

#include <functional>

#include "Network.hpp"

void Network::ProcessStreamOut(NetworkPacket* packet)
{

	uint32_t numEntities;
	packet->Read(numEntities);

	for (uint32_t i = 0; i < numEntities; i++) {

		std::uint32_t EntityHandle;

		packet->Read(EntityHandle);
	
		{
			
			
			{
				std::lock_guard<std::mutex> guard(streamLock);

				auto idx = std::find_if(StreamedEntities.begin(), StreamedEntities.end(), [EntityHandle](Entity* s) {
					return s->EntityHandle == EntityHandle;
					});
				if (idx != StreamedEntities.end()) {
					EntityStreamOut(*idx);
				//	(*idx)->lagRecords.clear();
					StreamedEntities.erase(idx);
				}
			}

		}

		std::cout << "Stream OUT in entity handle " << (int)EntityHandle << std::endl;


	}

}
void Network::ProcessStream(NetworkPacket* packet)
{
	

	uint32_t numEntities;
	packet->Read(numEntities);




	for (uint32_t i = 0; i < numEntities; i++) {

		EntityStruct Serialized;

		packet->Read(Serialized.EntityHandle);
		packet->Read(Serialized.Type);
		packet->Read(Serialized.Position);


		std::cout << "Stream in entity handle " << (int)Serialized.EntityHandle << std::endl;
	
		if (Serialized.Type == EntityType::ET_Player)
		{
			auto player = new Player(Serialized);
			{
				std::lock_guard<std::mutex> guard(streamLock);
				StreamedEntities.push_back(player);
				RenderedEntities.push_back(player);

			}
			player->Position = Serialized.Position;
	

			EntityStreamIn(player);
		}
		else if (Serialized.Type == EntityType::ET_Ped)
		{
			auto ped = new Ped(Serialized);
			{
				std::lock_guard<std::mutex> guard(streamLock);
				StreamedEntities.push_back(ped);
				RenderedEntities.push_back(ped);

			}
			ped->Position = Serialized.Position;
			

			EntityStreamIn(ped);
		}
	
		
	}
}

