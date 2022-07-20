#pragma once

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

class InteractionWorld : public ECSListener
{
public:
	InteractionWorld(ECS& ecsIn) : ECSListener(), ecs(ecsIn), compareAABB(ecsIn, 0)
	{
		setNotificationSettings(true, false);
		addComponentID(TransformComponent::ID);
		addComponentID(ColliderComponent::ID);
		addComponentID(CollisionTypeComponent::ID);

		DEBUG_LOG_TEMP("%d", collisionTable.size());
	}
	virtual void onMakeEntity(EntityHandle handle);
	virtual void onRemoveEntity(EntityHandle handle);
	virtual void onAddComponent(EntityHandle handle, uint32_t id);
	virtual void onRemoveComponent(EntityHandle handle, uint32_t id);

	void processInteractions(float delta);
	// static void addCollision(CollisionTypeComponent first, CollisionTypeComponent second, CollisionResponse response);

	void addInteraction(Interaction* interaction);

protected:
	void processInteractionInternal(float delta, size_t interactorIndex, size_t interacteeIndex,
		Array<BaseECSComponent*>& interactorComponents, Array<BaseECSComponent*>& interacteeComponents);
private:
	struct EntityInternal {
		EntityHandle handle;
		Array<uint32_t> interactors;
		Array<uint32_t> interactees;
	};

	struct InteractionWorldCompare {
		uint32_t axis;
		ECS& ecs;

		InteractionWorldCompare(ECS& ecsIn, uint32_t axisIn) :
			axis(axisIn), ecs(ecsIn) {}
		bool operator()(EntityInternal& a, EntityInternal& b) const {
			float aMin = ecs.getComponent<ColliderComponent>(a.handle)->transformedAABB.getMinExtents()[axis];
			float bMin = ecs.getComponent<ColliderComponent>(b.handle)->transformedAABB.getMinExtents()[axis];
			return (aMin < bMin);
		}
	};

	Array<EntityInternal> entities;
	Array<EntityHandle> entitiesToRemove;
	Array<EntityHandle> entitiesToUpdate;
	Array<Interaction*> interactions;
	ECS& ecs;
	InteractionWorldCompare compareAABB;

	void removeAndUpdateEntities();
	void addEntity(EntityHandle handle);

	void computeInteractions(EntityInternal& entity, uint32_t interactionIndex);

	static Map<CollisionTypePair, CollisionResponse> collisionTable;
};