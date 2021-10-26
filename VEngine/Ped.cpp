#include "pch.h"
#include <Shared.hpp>
#include "Implementations.hpp"
#include "nativeList.hpp"


void MULTIPLAYER::GAME_PED::GetAnimalAnimation(int modelhash, int movement, std::string* dict, std::string* name)
{
#pragma region Animals
	switch (modelhash)
	{
	case -832573324:
		*dict = "creatures@boar@move";
		if (movement == 1) //walking
		{
			*name = "walk";
		}
		else if (movement == 2) //running
		{
			*name = "trot";
		}
		else if (movement == 3) //sprinting
		{
			*name = "gallop";
		}
		else if (movement == 0) //idle
		{
			*name = "idle";
		}
		break;
	case 1462895032:
		*dict = "creatures@cat@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "canter";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -1430839454:
		*dict = "creatures@chickenhawk@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "glide";
		}
		else if (movement == 3)
		{
			*name = "flapping";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 51016938:
	case 1126154828:
		*dict = "creatures@dog@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "run";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 1457690978:
		*dict = "creatures@cormorant@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "glide";
		}
		else if (movement == 3)
		{
			*name = "flapping";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -50684386:
		*dict = "creatures@cow@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "trot";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 1682622302:
		*dict = "creatures@coyote@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "trot";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 402729631:
		*dict = "creatures@crow@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "glide";
		}
		else if (movement == 3)
		{
			*name = "flapping";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -664053099:
		*dict = "creatures@deer@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "trot";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -1950698411:
		*dict = "creatures@dolphin@move";
		if (movement == 1)
		{
			*name = "swim";
		}
		else if (movement == 2)
		{
			*name = "accelerate";
		}
		else if (movement == 3)
		{
			*name = "accelerate";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 802685111:
		*dict = "creatures@fish@move";
		if (movement == 1)
		{
			*name = "swim";
		}
		else if (movement == 2)
		{
			*name = "accelerate";
		}
		else if (movement == 3)
		{
			*name = "accelerate";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 1794449327:
		*dict = "creatures@hen@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "run";
		}
		else if (movement == 3)
		{
			*name = "run";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 1193010354:
		*dict = "creatures@humpback@move";
		if (movement == 1)
		{
			*name = "swim";
		}
		else if (movement == 2)
		{
			*name = "accelerate";
		}
		else if (movement == 3)
		{
			*name = "accelerate";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 1318032802:
		*dict = "creatures@husky@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "canter";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -1920284487:
		*dict = "creatures@killerwhale@move";
		if (movement == 1)
		{
			*name = "swim";
		}
		else if (movement == 2)
		{
			*name = "accelerate";
		}
		else if (movement == 3)
		{
			*name = "accelerate";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -1323586730:
		*dict = "creatures@pig@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "trot";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -745300483:
	case 111281960:
		*dict = "creatures@pigeon@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "glide";
		}
		else if (movement == 3)
		{
			*name = "flapping";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 1125994524:
	case 1832265812:
	case -1384627013:
		*dict = "creatures@pug@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "canter";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -541762431:
		*dict = "creatures@rabbit@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "canter";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -1011537562:
		*dict = "creatures@rat@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "trot";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 882848737:
		*dict = "creatures@retriever@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "canter";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -1788665315:
		*dict = "creatures@rottweiler@move";
		if (movement == 1)
		{
			*name = "walk";
		}
		else if (movement == 2)
		{
			*name = "canter";
		}
		else if (movement == 3)
		{
			*name = "gallop";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case 1015224100:
	case 113504370:
		*dict = "creatures@shark@move";
		if (movement == 1)
		{
			*name = "swim";
		}
		else if (movement == 2)
		{
			*name = "glide";
		}
		else if (movement == 3)
		{
			*name = "accelerate";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
	case -1589092019:
		*dict = "creatures@stingray@move";
		if (movement == 1)
		{
			*name = "swim";
		}
		else if (movement == 2)
		{
			*name = "accelerate";
		}
		else if (movement == 3)
		{
			*name = "accelerate";
		}
		else if (movement == 0)
		{
			*name = "idle";
		}
		break;
#pragma endregion Animals Animation Dictionaries
#pragma region Female

#pragma endregion Female Animation Dictionaries
	default:
		return;
	}
}
bool MULTIPLAYER::GAME_PED::IsPedAnimal(Ped* ped)
{
	switch (ped->Model)
	{
	case -832573324:
	case 1462895032:
	case -1430839454:
	case 51016938:
	case 1126154828:
	case 1457690978:
	case -50684386:
	case 1682622302:
	case 402729631:
	case -664053099:
	case -1950698411:
	case 802685111:
	case 1794449327:
	case 1193010354:
	case 1318032802:
	case -1920284487:
	case -1323586730:
	case 111281960:
	case 1125994524:
	case 1832265812:
	case -1384627013:
	case -541762431:
	case -1011537562:
	case 882848737:
	case -1788665315:
	case -745300483:
	case 1015224100:
	case 113504370:
	case -1589092019:
		return true;
	default:
		return false;
	}
}

void MULTIPLAYER::GAME_PED::UpdatePedWalkingAnimation(Ped* ped)
{
	if (IsPedAnimal(ped))
	{
		std::string dict;
		std::string name;

		if (ped->ForwardSpeed < 2.0f && ped->ForwardSpeed > 1.0f && ped->PedMoveType != 1)
		{
			GetAnimalAnimation(ped->Model, 1, &dict, &name);

			if (!STREAMING::HAS_ANIM_DICT_LOADED((char*)dict.c_str()))
				STREAMING::REQUEST_ANIM_DICT((char*)dict.c_str());

			BRAIN::TASK_PLAY_ANIM(ped->GameHandle, (char*)dict.c_str(), (char*)name.c_str(), 8.0f, 0.0f, -1, 1, 0.0f, false, false, false);
			PED::FORCE_PED_MOTION_STATE(ped->GameHandle, GAMEPLAY::GET_HASH_KEY("motionstate_walk"), false, false, false);
			ped->PedMoveType = 1;
		}
		else if (ped->ForwardSpeed > 2.0f && ped->ForwardSpeed <= 5.2f && ped->PedMoveType != 2)
		{
			GetAnimalAnimation(ped->Model, 2, &dict, &name);

			if (!STREAMING::HAS_ANIM_DICT_LOADED((char*)dict.c_str()))
				STREAMING::REQUEST_ANIM_DICT((char*)dict.c_str());

			BRAIN::TASK_PLAY_ANIM(ped->GameHandle, (char*)dict.c_str(), (char*)name.c_str(), 8.0f, 0.0f, -1, 1, 0.0f, false, false, false);
			PED::FORCE_PED_MOTION_STATE(ped->GameHandle, GAMEPLAY::GET_HASH_KEY("motionstate_run"), false, false, false);
			ped->PedMoveType = 2;
		}
		else if (ped->ForwardSpeed > 5.2f && ped->PedMoveType != 3)
		{
			GetAnimalAnimation(ped->Model, 3, &dict, &name);

			if (!STREAMING::HAS_ANIM_DICT_LOADED((char*)dict.c_str()))
				STREAMING::REQUEST_ANIM_DICT((char*)dict.c_str());

			BRAIN::TASK_PLAY_ANIM(ped->GameHandle, dict.c_str(), name.c_str(), 8.0f, 0.0f, -1, 1, 0.0f, false, false, false);
			PED::FORCE_PED_MOTION_STATE(ped->GameHandle, -1115154469, false, false, false);
			ped->PedMoveType = 3;
		}
		else if (ped->ForwardSpeed < 1.0f && ped->PedMoveType != 0)
		{
			GetAnimalAnimation(ped->Model, 0, &dict, &name);

			if (!STREAMING::HAS_ANIM_DICT_LOADED((char*)dict.c_str()))
				STREAMING::REQUEST_ANIM_DICT((char*)dict.c_str());

			BRAIN::TASK_PLAY_ANIM(ped->GameHandle, dict.c_str(), name.c_str(), 8.0f, 0.0f, -1, 1, 0.0f, false, false, false);
			PED::FORCE_PED_MOTION_STATE(ped->GameHandle, GAMEPLAY::GET_HASH_KEY("motionstate_idle"), false, false, false);
			ped->PedMoveType = 0;
		}
	}
	else
	{
		if (!STREAMING::HAS_ANIM_DICT_LOADED("move_m@generic"))
			STREAMING::REQUEST_ANIM_DICT("move_m@generic");

		if (ped->ForwardSpeed < 2.0f && ped->ForwardSpeed > 1.0f && ped->PedMoveType != 1)
		{
			PED::FORCE_PED_MOTION_STATE(ped->GameHandle, GAMEPLAY::GET_HASH_KEY("motionstate_walk"), false, false, false);
			BRAIN::TASK_PLAY_ANIM(ped->GameHandle, "move_m@generic", "walk", 8.0f, 0.0f, -1, 1, 0.0f, false, false, false);
			ped->PedMoveType = 1;
		}
		else if (ped->ForwardSpeed > 2.0f && ped->ForwardSpeed <= 5.2f && ped->PedMoveType != 2)
		{
			PED::FORCE_PED_MOTION_STATE(ped->GameHandle, GAMEPLAY::GET_HASH_KEY("motionstate_run"), false, false, false);
			BRAIN::TASK_PLAY_ANIM(ped->GameHandle, "move_m@generic", "run", 8.0f, 0.0f, -1, 1, 0.0f, false, false, false);
			ped->PedMoveType = 2;
		}
		else if (ped->ForwardSpeed > 5.2f && ped->PedMoveType != 3)
		{
			PED::FORCE_PED_MOTION_STATE(ped->GameHandle, -1115154469, false, false, false);
			BRAIN::TASK_PLAY_ANIM(ped->GameHandle, "move_m@generic", "sprint", 8.0f, 0.0f, -1, 1, 0.0f, false, false, false);
			ped->PedMoveType = 3;
		}
		else if (ped->ForwardSpeed < 1.0f && ped->PedMoveType != 0)
		{
			PED::FORCE_PED_MOTION_STATE(ped->GameHandle, GAMEPLAY::GET_HASH_KEY("motionstate_idle"), false, false, false);
			BRAIN::TASK_PLAY_ANIM(ped->GameHandle, "move_m@generic", "idle", 8.0f, 0.0f, -1, 1, 0.0f, false, false, false);
			ped->PedMoveType = 0;
		}
	}
}