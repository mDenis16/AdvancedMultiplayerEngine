#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/epsilon.hpp>
class LagRecord
{
public:
	glm::vec3 Position = glm::vec3(0, 0, 0);
	glm::vec3 Velocity = glm::vec3(0, 0, 0);
	glm::vec3 RawVelocity = glm::vec3(0, 0, 0);

	float HeadingShadow = 0.f;
	float Heading = 0.f;
	float HeadingSpeed = 0.f;

	float DesiredMoveBlendRatio = 0.f;
	bool Jumping = false;

	int cellIndex = 0;
	int MoveType = 0;
	float ForwardSpeed = 0;

	std::uint32_t tickCount = 0;
	std::uint32_t clientTickcount = 0;
	std::uint32_t tickRate = 0;


	unsigned int delayTime;
	
};
