#include "interactionWorld.hpp"
#include <algorithm>

void InteractionWorld::onMakeEntity(EntityHandle handle)
{
	addEntity(handle);
}

void InteractionWorld::onRemoveEntity(EntityHandle handle)
{
	entitiesToRemove.push_back(handle);
}

void InteractionWorld::onAddComponent(EntityHandle handle, uint32_t id)
{
	Array<uint32_t> componentIds = getComponentIDs();
	if(std::find(componentIds.begin(), componentIds.end(), id) != componentIds.end())
	{
		if(ecs.hasAllComponents(handle, componentIds))
		{
			addEntity(handle);
		}
	}
	else if(ecs.hasAllComponents(handle, componentIds)) {
		entitiesToUpdate.push_back(handle);
	}
}

void InteractionWorld::onRemoveComponent(EntityHandle handle, uint32_t id)
{
	if(ecs.hasAnyComponent(handle, getComponentIDs())) {
		entitiesToRemove.push_back(handle);
	} else if(ecs.hasAllComponents(handle, getComponentIDs())) {
		entitiesToUpdate.push_back(handle);
	}
}

void InteractionWorld::addEntity(EntityHandle handle)
{
	EntityInternal entity;
	entity.handle = handle;
	for(size_t i = 0; i < interactions.size(); i++) {
		computeInteractions(entity, i);
	}
	entities.push_back(entity);
}

// void InteractionWorld::addCollision(CollisionTypeComponent first, CollisionTypeComponent second,
// 	CollisionResponse response)
// {
// 	collisionTable[{first, second}] = response;
// }

void InteractionWorld::addInteraction(Interaction* interaction) {
	interactions.push_back(interaction);
	size_t index = interactions.size()-1;
	for (auto& entity : entities)
	{
		computeInteractions(entity, index);
	}
}

void InteractionWorld::computeInteractions(EntityInternal& entity, uint32_t interactionIndex)
{
	Interaction* interaction = interactions[interactionIndex];
	bool isInteractor = true;
	for (uint32_t componentId : interaction->getInteractorComponents())
	{
		if(!ecs.hasComponent(entity.handle, componentId)) {
			isInteractor = false;
			break;
		}
	}
	bool isInteractee = true;
	for (uint32_t componentId : interaction->getInteracteeComponents())
	{
		if(!ecs.hasComponent(entity.handle, componentId)) {
			isInteractee = false;
			break;
		}
	}
	if(isInteractor) {
		entity.interactors.push_back(interactionIndex);
	}
	if(isInteractee) {
		entity.interactees.push_back(interactionIndex);
	}
}

void InteractionWorld::processInteractionInternal(float delta, size_t interactorIndex, size_t interacteeIndex,
	Array<BaseECSComponent*>& interactorComponents, Array<BaseECSComponent*>& interacteeComponents)
{
	for(size_t dummyIndex = 0; dummyIndex < 2; dummyIndex++) {
		for(size_t k = 0; k < entities[interactorIndex].interactors.size(); k++) {
			for(size_t l = 0; l < entities[interacteeIndex].interactees.size(); l++) {
				uint32_t index = entities[interactorIndex].interactors[k];
				if(index == entities[interacteeIndex].interactees[l]) {
					Interaction* interaction = interactions[index];
					interactorComponents.resize(Math::max(interactorComponents.size(), interaction->getInteractorComponents().size()));
					interacteeComponents.resize(Math::max(interacteeComponents.size(), interaction->getInteracteeComponents().size()));
					for(size_t m = 0; m < interaction->getInteractorComponents().size(); m++) {
						interactorComponents[m] = ecs.getComponentByType(entities[interactorIndex].handle, interaction->getInteractorComponents()[m]);	
					}
					for(size_t m = 0; m < interaction->getInteracteeComponents().size(); m++) {
						interacteeComponents[m] = ecs.getComponentByType(entities[interacteeIndex].handle, interaction->getInteracteeComponents()[m]);	
					}
					interaction->interact(delta, &interactorComponents[0], &interacteeComponents[0]);
				}
			}
		}
		// Check the other possibility: If the first entity is the interactee instead of the interactor
		size_t tempIndex = interactorIndex;
		interactorIndex = interacteeIndex;
		interacteeIndex = tempIndex;
	}
}

void InteractionWorld::processInteractions(float delta)
{
	removeAndUpdateEntities();
	for (auto& entity : entities)
	{
		ColliderComponent* colliderComponent = ecs.getComponent<ColliderComponent>(entity.handle);
		colliderComponent->transformedAABB = colliderComponent->aabb.transform(
				ecs.getComponent<TransformComponent>(entity.handle)->transform.toMatrix());
	}
	std::sort(entities.begin(), entities.end(), compareAABB);
	// Go through the list, test intersections in range
	Array<BaseECSComponent*> interactorComponents;
	Array<BaseECSComponent*> interacteeComponents;
	Vector3f centerSum(0.0f);
	Vector3f centerSqSum(0.0f);
	for(size_t i = 0; i < entities.size(); i++) {
		EntityInternal firstEntity = entities[i];
		if(ecs.getComponent<CollisionTypeComponent>(firstEntity.handle)->collisionType == CollisionType::NONE)
		{
			continue;
		}
		AABB aabb = ecs.getComponent<ColliderComponent>(firstEntity.handle)->transformedAABB;
		Vector3f center = aabb.getCenter();
		centerSum += center;
		centerSqSum += (center * center);
		
		// Find intersections for this entity
		for(size_t j = i+1; j < entities.size(); j++) {
			EntityInternal secondEntity = entities[j];
			AABB otherAABB = ecs.getComponent<ColliderComponent>(secondEntity.handle)->transformedAABB;
			if(otherAABB.getMinExtents()[compareAABB.axis]
					> aabb.getMaxExtents()[compareAABB.axis]) {
				break;
			}

			// we only check intersections if entities' collision types allow it
			CollisionType collisionTypeFirst = ecs.getComponent<CollisionTypeComponent>(firstEntity.handle)->collisionType;
			CollisionType collisionTypeSecond = ecs.getComponent<CollisionTypeComponent>(secondEntity.handle)->collisionType;
			if(collisionTable[{collisionTypeFirst, collisionTypeSecond}] == CollisionResponse::NONE)
			{
				continue;
			}

			if(aabb.intersects(otherAABB) && collisionTable[{collisionTypeFirst, collisionTypeSecond}] == CollisionResponse::BLOCK) {
				// size_t interactorIndex = i;
				// size_t interacteeIndex = j;
				processInteractionInternal(delta, i, j, interactorComponents, interacteeComponents);
			}
		}
	}

	// Set max variance axis
	centerSum /= entities.size();
	centerSqSum /= entities.size();
	Vector3f variance = centerSqSum - (centerSum*centerSum);
	float maxVar = variance[0];
	uint32_t maxVarAxis = 0;
	if(variance[1] > maxVar) {
		maxVar = variance[1];
		maxVarAxis = 1;
	}
	if(variance[2] > maxVar) {
		maxVar = variance[2];
		maxVarAxis = 2;
	}
	compareAABB.axis = maxVarAxis;
}

void InteractionWorld::removeAndUpdateEntities()
{
	if(entitiesToRemove.size() == 0) {
		return;
	}
	for(size_t i = 0; i < entities.size(); i++) {
		// Remove entities
		bool didRemove = false;
		do {
			didRemove = false;
			for(size_t j = 0; j < entitiesToRemove.size(); j++) {
				if(entities[i].handle == entitiesToRemove[j]) {
					utils::swap_remove(entities, i);
					utils::swap_remove(entitiesToRemove, j);
					didRemove = true;
					break;
				}
			}
			if(entitiesToRemove.size() == 0 && entitiesToUpdate.size() == 0) {
				return;
			}
		} while(didRemove);

		// Update entities
		for(size_t j = 0; j < entitiesToUpdate.size(); j++) {
			if(entities[i].handle == entitiesToUpdate[j]) {
				entities[i].interactors.clear();
				entities[i].interactees.clear();
				for(size_t k = 0; k < interactions.size(); k++) {
					computeInteractions(entities[i], k);
				}
				utils::swap_remove(entitiesToUpdate, j);
			}
		}
	}
	entitiesToRemove.clear();
	entitiesToUpdate.clear();
}

