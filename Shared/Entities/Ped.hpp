#pragma once


enum EPedMoveType
{
	OFMT_Still,
	OFMT_Walking,
	OFMT_Running,
	OFMT_Sprinting
};


class Ped : public Entity {
public:

	int PedMoveType;
	float ForwardSpeed = 0.f;
	
	int LagCompTick = 0;
	
	void CreateMove(NetworkPacket* packet) override;


};
