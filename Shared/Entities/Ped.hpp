#pragma once


class Player;

enum EPedMoveType
{
	OFMT_Still,
	OFMT_Walking,
	OFMT_Running,
	OFMT_Sprinting
};


class Ped : public Entity {
public:
#if SERVER

	Ped(std::uint32_t Handle);
#endif
	Ped() {};
	Ped(EntityStruct& ent);

	glm::vec3 MoveError = glm::vec3(0, 0, 0);


	int PedMoveType;
	float ForwardSpeed = 0.f;

	




	int LagCompTick = 0;
	bool _lastJumping = false;
	bool _lastForwardSpeed = 0.f;
	void CreateMove(NetworkPacket* packet) override;

	Player* Controller = nullptr;
};
