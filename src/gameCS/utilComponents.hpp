#pragma once

#include "ecs/ecs.hpp"
#include "math/transform.hpp"
#include "math/aabb.hpp"

struct TransformComponent : public ECSComponent<TransformComponent>
{
	Transform transform;
};

inline std::ostream& operator<<(std::ostream& os, const TransformComponent& value)
{
	os << std::setw(ECS_LOG_MARGIN) << StringFuncs::toString(value.transform);
	return os;
}

ECS_COMPONENT_PRINT(TransformComponent, "transform");

