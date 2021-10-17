#pragma once

class Client {
public:

	Network network;


	virtual void OnFrameStage();
	virtual void OnTick();

};