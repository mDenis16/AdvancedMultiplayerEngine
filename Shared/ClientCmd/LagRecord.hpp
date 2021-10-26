#pragma once


class LagRecord
{
public:
	glm::vec3 Position = glm::vec3(0, 0, 0);
	glm::vec3 Velocity = glm::vec3(0, 0, 0);
	glm::vec3 Rotation = glm::vec3(0,0,0);

	float Heading = 0.f;

	int cellIndex = 0;
	int MoveType = 0;

	float ForwardSpeed = 0;

	
	ClockTime simulationTime;
	int tickcount = 0;
};
