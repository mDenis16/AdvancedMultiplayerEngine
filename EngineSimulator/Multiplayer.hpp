#pragma once


class MultiplayerNetwork : public Network {
public:
	

	void EventDisconnect() override;
	void EventConnect() override;
	void HandlePacket(NetworkPacket* packet) override;

	void EntityStreamIn(Entity* ent) override;
	void EntityStreamOut(Entity* ent) override;

	void OnCreateMove();

	ClockTime lastCreateMove;

};

inline MultiplayerNetwork Multiplayer;
