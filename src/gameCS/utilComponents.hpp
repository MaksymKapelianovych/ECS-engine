#pragma once

#include "ecs/ecs.hpp"
#include "math/transform.hpp"
#include "math/aabb.hpp"

struct TransformComponent : public ECSComponent<TransformComponent>
{
	Transform transform;
};

