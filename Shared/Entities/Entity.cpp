#include "Common.hpp"
#include "Shared.hpp"

#include "Entity.hpp"



std::vector<unsigned char> Entity::Serialize()
{
	return std::vector<unsigned char>();
}

EntityRenderData Entity::Interpolation()
{
	EntityRenderData data = {};
	
	if (this->lagRecords.size() > 2) {

			auto current = this->lagRecords.back();

			auto previous = this->lagRecords.at(this->lagRecords.size() - 2);

			float progress = (float)ClockDuration(Clock::now() - current.simulationTime).count() / (float)ClockDuration(current.simulationTime - previous.simulationTime).count();
			progress = std::clamp(progress, 0.f, 1.f);
			data.Velocity = glm::mix(previous.Velocity, current.Velocity, std::clamp(progress, 0.f, 1.f));
			data.Position = glm::mix(previous.Position, current.Position, std::clamp(progress, 0.f, 1.f));

			float radStart = glm::radians(previous.Heading);
			float radEnd = glm::radians(current.Heading);

			glm::vec3 up(0.0f, 0.0f, 1.0f);
			
			data.Quaternion = glm::mix(glm::angleAxis(radStart, up), glm::angleAxis(radEnd, up), std::clamp(progress, 0.f, 1.f));


	}else
	{
		data.Position = this->Position;
		data.Rotation = this->Rotation;
		data.Velocity = this->Velocity;
		data.Heading = this->Heading;
	}

	return data;
}
