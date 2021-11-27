#include "Common.hpp"
#include "Shared.hpp"

#include "Entity.hpp"
#if CLIENT
#include "..\..\Client\Network.hpp"
#endif
#if SERVER
#include "..\..\Server/Network.h" 
#endif

std::vector<unsigned char> Entity::Serialize()
{
	return std::vector<unsigned char>();
}


float LerpDegrees(float a, float b, float lerpFactor) // Lerps from angle a to b (both between 0.f and 360.f), taking the shortest path
{
	float result;
	float diff = b - a;
	if (diff < -180.f)
	{
		// lerp upwards past 360
		b += 360.f;
		result = std::lerp(a, b, lerpFactor);
		if (result >= 360.f)
		{
			result -= 360.f;
		}
	}
	else if (diff > 180.f)
	{
		// lerp downwards past 0
		b -= 360.f;
		result = std::lerp(a, b, lerpFactor);
		if (result < 0.f)
		{
			result += 360.f;
		}
	}
	else
	{
		// straight lerp
		result = std::lerp(a, b, lerpFactor);
	}

	return result;
}
glm::vec3 Entity::Interpolation(float time)
{
	if (this->lagRecords.size() > 2) {
		/*for (auto& l : lagRecords)
		{

			std::cout << "Simulation time " << ClockDuration(_pServer->m_tmStarted - l->simulationTime).count() <<  " extrapolated " << l->is_simulated << std::endl;

		}*/
		auto current = this->lagRecords.back();

		auto previous = this->lagRecords.at(this->lagRecords.size() - 2);

		float progress = (float)(Network::CurrentFrameTime - current->tickCount) / (float)(current->tickCount - previous->tickCount);

		//std::cout << "progress " << progress << "RECORD TICKRATE" << Network::CurrentFrameTime - current->tickCount << std::endl;

		return glm::mix(previous->Position, current->Position, glm::clamp(progress, 0.f, 1.f));
	}

	return glm::vec3();
}