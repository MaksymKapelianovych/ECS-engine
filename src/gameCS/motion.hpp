#pragma once

#include <iomanip>
#include <iostream>
#include "log_utils.h"
#include "ecs/ecs.hpp"
#include "utilComponents.hpp"
#include "motionIntegrators.hpp"

struct MotionComponent : public ECSComponent<MotionComponent>
{
	Vector3f velocity = Vector3f(0.0f, 0.0f, 0.0f);
	Vector3f acceleration = Vector3f(0.0f, 0.0f, 0.0f);
};

inline std::ostream& operator<<(std::ostream& os, const MotionComponent& value)
{
	os << std::setw(ECS_LOG_MARGIN) << std::setfill(' ') << StringFuncs::toString(value.velocity);
	os << std::setw(ECS_LOG_MARGIN) << std::setfill(' ') << StringFuncs::toString(value.acceleration);
	return os;
}

ECS_COMPONENT_PRINT(MotionComponent, "velocity", "acceleration");


class MotionSystem : public BaseECSSystem
{
public:
	MotionSystem(ECS& ecsIn) : BaseECSSystem(ecsIn)
	{
		addComponentType(TransformComponent::ID);
		addComponentType(MotionComponent::ID);
	}

	virtual void updateComponents(float delta, BaseECSComponent** components)
	{
		TransformComponent* transform = (TransformComponent*)components[0];
		MotionComponent* motion = (MotionComponent*)components[1];

		// std::cout << "MotionSystem   " << ecs.handleToEntityIndex(transform->entity) << "   " << ecs.handleToEntityIndex(motion->entity) << std::endl;
		
		Vector3f newPos = transform->transform.getTranslation();
		MotionIntegrators::forestRuth(newPos, motion->velocity, motion->acceleration, delta);
		transform->transform.setTranslation(newPos);

		// TODO: This is a cheap hack to get resistance for now! Replace with a proper system later
		motion->acceleration = motion->acceleration * 0.8f;
		motion->velocity = motion->velocity * 0.95f;
	}
};

