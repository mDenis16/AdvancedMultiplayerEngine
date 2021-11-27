#pragma once

#include <thread>

#include <Shared/Shared.hpp>
#include <Gridmap/Grid.hpp>


class SceneView
{
public:
	void Init();

	glm::vec3 localPosition = glm::vec3(0,0,0);
	float localHeading = 0.f;

	GridCell<Entity*>* localCell;



	std::thread* renderThread;
	void OnRender();
};
inline SceneView scene;
