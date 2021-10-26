#pragma once
#include <Entities/Ped.hpp>

namespace MULTIPLAYER
{
	namespace GAME_PED
	{
		void UpdatePedWalkingAnimation(Ped* ped);
		bool IsPedAnimal(Ped* ped);

		void GetAnimalAnimation(int modelhash, int movement, std::string* dict, std::string* name);

	}
}
