#pragma once
#include "ecs/ecs.hpp"
#include "math/aabb.hpp"

struct ColliderComponent : public ECSComponent<ColliderComponent>
{
    AABB aabb;
    AABB transformedAABB;
};

enum class CollisionResponse : uint8_t
{
    BLOCK,
    OVERLAP,
    NONE
};



enum class CollisionType: uint8_t
{
    STATIC,
    MOVABLE,
    PLAYER,
    NONE, // default collision type if any explicitly set

    NUM // DO NOT USE THIS, ADD NEW COLLISION TYPES BEFORE
};


struct CollisionTypeComponent : public ECSComponent<CollisionTypeComponent>
{
    CollisionTypeComponent(CollisionType type) : collisionType(type) {}
    // Change later to uint8_t so collision type will be able to be added at runtime or from .ini file
    CollisionType collisionType;

};

// Helper struct to facilitate detecting same pair in collision table
struct CollisionTypePair
{
    CollisionType first;
    CollisionType second;

    friend bool operator<(const CollisionTypePair& lhs, const CollisionTypePair& rhs)
    {
        return lhs.first < rhs.first || (!(rhs.first < lhs.first) && lhs.second < rhs.second);
    }
};


