#pragma once

#include <thread>

#include <Shared/Shared.hpp>
#include <Gridmap/Grid.hpp>


class SceneView
{
public:
	void Init();

	glm::vec3 localPosition = glm::vec3(0,0,0);

	GridCell<Entity*>* localCell;



	std::thread* renderThread;
	void OnRender();
};
inline SceneView scene;
