#pragma once
#include "pch.h"
#include <Shared.hpp>
#include <Client/Network.hpp>

class MultiplayerNetwork : public Network {
public:


	void EventDisconnect() override;
	void EventConnect() override;
	void HandlePacket(NetworkPacket* packet) override;

	void EntityStreamIn(Entity* ent) override;
	void EntityStreamOut(Entity* ent) override;

	void OnEntityCreateMove(Entity* entity) override;
	void OnCreateMove();

	ClockTime lastCreateMove;

};

inline MultiplayerNetwork Multiplayer;
