#pragma once

#include "motion.hpp"
#include "ecs/ecs.hpp"
#include "gameCS/utilComponents.hpp"
#include "gameCS/collision.h"

class Interaction
{
public:
	virtual void interact(float delta, BaseECSComponent** interactorComponents, BaseECSComponent** interacteeComponents) {}

	const Array<uint32_t>& getInteractorComponents() const {
		return interactorComponentTypes;
	}
	const Array<uint32_t>& getInteracteeComponents() const {
		return interacteeComponentTypes;
	}
protected:
	void addInteractorComponentType(uint32_t type) {
		interactorComponentTypes.push_back(type);
	}
	void addInteracteeComponentType(uint32_t type) {
		interacteeComponentTypes.push_back(type);
	}
private:
	Array<uint32_t> interactorComponentTypes;
	Array<uint32_t> interacteeComponentTypes;
};


// class OverlapInteraction : public Interaction
// {
// public:
// 	OverlapInteraction() : Interaction()
// 	{
// 		addInteractorComponentType(TransformComponent::ID);
// 		addInteractorComponentType(ColliderComponent::ID);
// 		addInteractorComponentType(MotionComponent::ID);
// 		addInteractorComponentType(CollisionTypeComponent::ID);
// 		
// 		addInteracteeComponentType(TransformComponent::ID);
// 		addInteracteeComponentType(ColliderComponent::ID);
// 		addInteracteeComponentType(MotionComponent::ID);
// 		addInteracteeComponentType(CollisionTypeComponent::ID);
// 	}
//
// 	virtual void interact(float delta, BaseECSComponent** interactorComponents, BaseECSComponent** interacteeComponents)
// 	{
// 		Transform& transformInteractor = ((TransformComponent*)interactorComponents[0])->transform;
// 		Transform& transformInteractee = ((TransformComponent*)interacteeComponents[0])->transform;
// 		
// 		ColliderComponent* colliderInteractor = ((ColliderComponent*)interactorComponents[1]);
// 		ColliderComponent* colliderInteractee = ((ColliderComponent*)interacteeComponents[1]);
//
// 		MotionComponent* motionInteractor = ((MotionComponent*)interactorComponents[2]);
// 		MotionComponent* motionInteractee = ((MotionComponent*)interacteeComponents[2]);
// 		
// 		CollisionTypeComponent* collisionInteractor = ((CollisionTypeComponent*)interactorComponents[3]);
// 		CollisionTypeComponent* collisionInteractee = ((CollisionTypeComponent*)interacteeComponents[3]);
//
// 		String collision1;
// 		String collision2;
//
// 		switch (collisionInteractor->collisionType) {
// 			case CollisionType::STATIC: collision1 = "static"; break;
// 			case CollisionType::MOVABLE: collision1 = "movable"; break;
// 			case CollisionType::PLAYER: collision1 = "player"; break;
// 			case CollisionType::NONE: collision1 = "none"; break;
// 			case CollisionType::NUM: break;
// 			default: ;
// 		}
// 		
// 		switch (collisionInteractee->collisionType) {
// 			case CollisionType::STATIC: collision2 = "static"; break;
// 			case CollisionType::MOVABLE: collision2 = "movable"; break;
// 			case CollisionType::PLAYER: collision2 = "player"; break;
// 			case CollisionType::NONE: collision2 = "none"; break;
// 			case CollisionType::NUM: break;
// 			default: ;
// 		}
//
// 		DEBUG_LOG("log_interaction", LOG_WARNING, "Interacting entity (%d:%s) with (%d:%s) ",
// 			ECS::handleToRawType(colliderInteractor->entity)->first, collision1.c_str(), ECS::handleToRawType(colliderInteractee->entity)->first, collision2.c_str());
//
// 		const Vector3f distanceVec = colliderInteractor->aabb.distanceTo(colliderInteractee->aabb);
// 		const float distance = distanceVec.length();
//
// 		Vector3f velocity = motionInteractor->velocity;
//
// 		transformInteractor.setTranslation(transformInteractor.getTranslation() - motionInteractor->velocity * delta);
// 		transformInteractee.setTranslation(transformInteractee.getTranslation() - motionInteractee->velocity * delta);
//
// 		float xAxisTimeToCollide = !Math::isNearZero(velocity.getX()) ? Math::abs(distance / (velocity.getX() * delta)) : std::numeric_limits<float>::infinity();
// 		float yAxisTimeToCollide = !Math::isNearZero(velocity.getY()) ? Math::abs(distance / (velocity.getY() * delta)) : std::numeric_limits<float>::infinity();
// 		float zAxisTimeToCollide = !Math::isNearZero(velocity.getZ()) ? Math::abs(distance / (velocity.getZ() * delta)) : std::numeric_limits<float>::infinity();
//
// 		float shortestTime = Math::min3(xAxisTimeToCollide, yAxisTimeToCollide, zAxisTimeToCollide);
// 		
// 		transformInteractor.setTranslation(transformInteractor.getTranslation() - velocity * shortestTime * delta);
//
// 		// motionInteractor->velocity *= -0.5f;
// 		// motionInteractee->velocity *= -0.5f;
//
// 		// motionComponent->velocity = motionComponent->velocity * -1.1f;
// 	}
// };

// class InteractionWorld : public ECSListener
// {
// public:
// 	InteractionWorld(ECS& ecsIn) : ECSListener(), ecs(ecsIn), compareAABB(ecsIn, 0)
// 	{
// 		setNotificationSettings(true, false);
// 		addComponentID(TransformComponent::ID);
// 		addComponentID(ColliderComponent::ID);
// 		addComponentID(CollisionTypeComponent::ID);
//
// 		DEBUG_LOG_TEMP("%d", collisionTable.size());
// 	}
// 	virtual void onMakeEntity(EntityHandle handle);
// 	virtual void onRemoveEntity(EntityHandle handle);
// 	virtual void onAddComponent(EntityHandle handle, uint32_t id);
// 	virtual void onRemoveComponent(EntityHandle handle, uint32_t id);
//
// 	void processInteractions(float delta);
// 	// static void addCollision(CollisionTypeComponent first, CollisionTypeComponent second, CollisionResponse response);
//
// 	void addInteraction(Interaction* interaction);
//
// protected:
// 	void processInteractionInternal(float delta, size_t interactorIndex, size_t interacteeIndex,
// 		Array<BaseECSComponent*>& interactorComponents, Array<BaseECSComponent*>& interacteeComponents);
// private:
// 	struct EntityInternal {
// 		EntityHandle handle;
// 		Array<uint32_t> interactors;
// 		Array<uint32_t> interactees;
// 	};
//
// 	struct InteractionWorldCompare {
// 		uint32_t axis;
// 		ECS& ecs;
//
// 		InteractionWorldCompare(ECS& ecsIn, uint32_t axisIn) :
// 			axis(axisIn), ecs(ecsIn) {}
// 		bool operator()(EntityInternal& a, EntityInternal& b) const {
// 			float aMin = ecs.getComponent<ColliderComponent>(a.handle)->transformedAABB.getMinExtents()[axis];
// 			float bMin = ecs.getComponent<ColliderComponent>(b.handle)->transformedAABB.getMinExtents()[axis];
// 			return (aMin < bMin);
// 		}
// 	};
//
// 	Array<EntityInternal> entities;
// 	Array<EntityHandle> entitiesToRemove;
// 	Array<EntityHandle> entitiesToUpdate;
// 	Array<Interaction*> interactions;
// 	ECS& ecs;
// 	InteractionWorldCompare compareAABB;
//
// 	void removeAndUpdateEntities();
// 	void addEntity(EntityHandle handle);
//
// 	void computeInteractions(EntityInternal& entity, uint32_t interactionIndex);
//
// 	static Map<CollisionTypePair, CollisionResponse> collisionTable;
// };