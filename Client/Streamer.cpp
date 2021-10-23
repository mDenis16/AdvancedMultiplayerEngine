
#include <Shared.hpp>

#include <functional>

#include "Network.hpp"

void Network::ProcessStreamOut(NetworkPacket* packet)
{
	std::cout << "Process stream out entities." << std::endl;

	uint32_t numEntities;
	packet->Read(numEntities);




	for (uint32_t i = 0; i < numEntities; i++) {

		Entity Serialized;

		packet->Read(Serialized.EntityHandle);
	


		std::cout << "Stream OUT in entity handle " << (int)Serialized.EntityHandle << std::endl;


	}

}
void Network::ProcessStream(NetworkPacket* packet)
{
	std::cout << "Process stream in entities." << std::endl;

	uint32_t numEntities;
	packet->Read(numEntities);




	for (uint32_t i = 0; i < numEntities; i++) {

		Entity Serialized;

		packet->Read(Serialized.EntityHandle);
		packet->Read(Serialized.Type);
		packet->Read(Serialized.Position);


		std::cout << "Stream in entity handle " << (int)Serialized.EntityHandle << std::endl;
	
		if (Serialized.Type == EntityType::ET_Player)
		{
			auto player = new Player(Serialized);

			StreamedEntities.push_back(player);
			EntityStreamIn(player);
		}
	
		
	}
}
