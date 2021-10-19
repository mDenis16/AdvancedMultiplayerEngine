#pragma once

#include <thread>

class SceneView
{
public:
	void Init();


	std::thread* renderThread;
	void OnRender();
};
