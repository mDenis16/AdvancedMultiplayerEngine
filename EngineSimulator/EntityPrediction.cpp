#include <Shared.hpp>

#include "EntityPrediction.h"
inline float TICKRATE = 70.f;


void EntityPrediction::OnPreRender(Entity* entity)
{

	if (entity->Velocity != glm::vec3(0, 0, 0)) {

		const glm::vec3 averageDir = (entity->Velocity + (entity->ShadowPosition - entity->Position)) / 2.f;

		glm::vec3 Direction = glm::normalize(averageDir);

		entity->Position += Direction * entity->EntitySpeed * TICKRATE;
	
	}

}
