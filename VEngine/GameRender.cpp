#include "pch.h"
#include <Shared.hpp>
#include <chrono>
#include "GameRender.hpp"
#include <VEngine\Engine.hpp>
#include <VEngine\Multiplayer.hpp>
using namespace Engine;

namespace GameRender {
	void Initialize()
	{
		_Thread = new std::thread(Render);
	}
	void Render()
	{
		std::cout << "Started game render thread " << std::endl;
		while (_Running) {

			
			std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 120));
		}
	}
	void Destruct()
	{
		_Running = false;
	}
}