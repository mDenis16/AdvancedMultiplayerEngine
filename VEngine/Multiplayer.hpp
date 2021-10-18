#pragma once


class MultiplayerNetwork : public Network {
public:
	void EventDisconnect() override;
	void EventConnect() override;
	void HandlePacket(NetworkPacket* packet) override;

};

//inline MultiplayerNetwork Multiplayer;
