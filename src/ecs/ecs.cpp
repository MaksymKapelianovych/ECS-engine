#include "ecs.hpp"

#include <algorithm>

#include "core/memory.hpp"
#include "math/math.hpp"

ECS::~ECS()
{
	for (auto& component : components)
	{
		size_t                   typeSize = BaseECSComponent::getTypeSize(component.first);
		ECSComponentFreeFunction freefn = BaseECSComponent::getTypeFreeFunction(component.first);
		for(uint32_t i = 0; i < component.second.size(); i += typeSize) {
			freefn((BaseECSComponent*)&component.second[i]);
		}
	}
	

	for (auto& entity : entities)
	{
		delete entity;
	}
}

EntityHandle ECS::makeEntity(BaseECSComponent* const * entityComponents, uint32_t const * componentIDs, size_t numComponents)
{
	std::pair<uint32_t, Array<std::pair<uint32_t, uint32_t> > >* newEntity = new std::pair<uint32_t, Array<std::pair<uint32_t, uint32_t> > >();
	EntityHandle handle = (EntityHandle)newEntity;
	for(uint32_t i = 0; i < numComponents; i++) {
		if(!BaseECSComponent::isTypeValid(componentIDs[i])) {
			DEBUG_LOG("ECS", LOG_ERROR, "'%u' is not a valid component type.", componentIDs[i]);
			delete newEntity;
			return NULL_ENTITY_HANDLE;
		}

		addComponentInternal(handle, newEntity->second, componentIDs[i], entityComponents[i]);
	}

	newEntity->first = entities.size();
	entities.push_back(newEntity);

	for (auto& listener : listeners)
	{
		bool isValid = true;
		if(listener->shouldNotifyOnAllEntityOperations()) {
			listener->onMakeEntity(handle);
		} else {
			for (unsigned int id : listener->getComponentIDs())
			{
				bool hasComponent = false;
				for(uint32_t k = 0; k < numComponents; k++) {
					if(id == componentIDs[k]) {
						hasComponent = true;
						break;
					}
				}
				if(!hasComponent) {
					isValid = false;
					break;
				}
			}
			if(isValid) {
				listener->onMakeEntity(handle);
			}
		}
	}

	return handle;
}

void ECS::removeEntity(EntityHandle handle)
{
	Array<std::pair<uint32_t, uint32_t> >& entity = handleToEntity(handle);

	for (auto& listener : listeners)
	{
		const Array<uint32_t>& componentIDs = listener->getComponentIDs();
		bool                   isValid = true;
		if(listener->shouldNotifyOnAllEntityOperations()) {
			listener->onRemoveEntity(handle);
		} else {
			for (unsigned int componentID : componentIDs)
			{
				bool hasComponent = false;
				for (auto& component : entity)
				{
					if(componentID == component.first) {
						hasComponent = true;
						break;
					}
				}
				if(!hasComponent) {
					isValid = false;
					break;
				}
			}
			if(isValid) {
				listener->onRemoveEntity(handle);
			}
		}
	}
	
	for (auto& component : entity)
	{
		deleteComponent(component.first, component.second);
	}

	uint32_t destIndex = handleToEntityIndex(handle);
	uint32_t srcIndex = entities.size() - 1;
	delete entities[destIndex];
	entities[destIndex] = entities[srcIndex];
	entities[destIndex]->first = destIndex;
	entities.pop_back();
}

void ECS::addComponentInternal(EntityHandle handle, Array<std::pair<uint32_t, uint32_t> >& entity, uint32_t componentID, BaseECSComponent* copyFrom)
{
	ECSComponentCreateFunction createfn = BaseECSComponent::getTypeCreateFunction(componentID);
	std::pair<uint32_t, uint32_t> newPair;
	newPair.first = componentID;
	newPair.second = createfn(components[componentID], handle, copyFrom);
	entity.push_back(newPair);
}

void ECS::deleteComponent(uint32_t componentID, uint32_t index)
{
	Array<uint8_t>& array = components[componentID];
	ECSComponentFreeFunction freefn = BaseECSComponent::getTypeFreeFunction(componentID);
	size_t typeSize = BaseECSComponent::getTypeSize(componentID);
	uint32_t srcIndex = array.size() - typeSize;

	BaseECSComponent* destComponent = (BaseECSComponent*)&array[index];
	BaseECSComponent* srcComponent = (BaseECSComponent*)&array[srcIndex];
	freefn(destComponent);

	if(index == srcIndex) {
		array.resize(srcIndex);
		return;
	}
	Memory::memcpy(destComponent, srcComponent, typeSize);

	Array<std::pair<uint32_t, uint32_t> >& srcComponents = handleToEntity(srcComponent->entity);
	for (auto& srcComponent : srcComponents)
	{
		if(componentID == srcComponent.first && srcIndex == srcComponent.second) {
			srcComponent.second = index;
			break;
		}
	}

	array.resize(srcIndex);
}

bool ECS::removeComponentInternal(EntityHandle handle, uint32_t componentID)
{
	Array<std::pair<uint32_t, uint32_t> >& entityComponents = handleToEntity(handle);
	for(auto& component : entityComponents) {
		if(componentID == component.first) {
			deleteComponent(component.first, component.second);
			component = std::move(entityComponents.back());
			entityComponents.pop_back();
			return true;
		}
	}
	// Previous code
	// for(uint32_t i = 0; i < entityComponents.size(); i++) {
	// 	if(componentID == entityComponents[i].first) {
	// 		deleteComponent(entityComponents[i].first, entityComponents[i].second);
	// 		uint32_t srcIndex = entityComponents.size()-1;
	// 		uint32_t destIndex = i;
	// 		entityComponents[destIndex] = entityComponents[srcIndex];
	// 		entityComponents.pop_back();
	// 		return true;
	// 	}
	// }
	return false;
}

BaseECSComponent* ECS::getComponentInternal(Array<std::pair<uint32_t, uint32_t> >& entityComponents, Array<uint8_t>& array, uint32_t componentID)
{
	for (auto& entityComponent : entityComponents)
	{
		if(componentID == entityComponent.first) {
			return (BaseECSComponent*)&array[entityComponent.second];
		}
	}
	return nullptr;
}

bool ECS::hasAllComponents(EntityHandle handle, Array<uint32_t> componentTypes)
{
	return std::all_of(componentTypes.begin(), componentTypes.end(),
		[&handle, this](uint32_t componentType)
		{
			return hasComponent(handle, componentType);
		});
}

bool ECS::hasAnyComponent(EntityHandle handle, Array<uint32_t> componentTypes)
{
	return std::any_of(componentTypes.begin(), componentTypes.end(),
		[&handle, this](uint32_t componentType)
		{
			return hasComponent(handle, componentType);
		});
}

void ECS::updateSystems(ECSSystemList& systems, float delta)
{
	Array<BaseECSComponent*> componentParam;
	Array<Array<uint8_t>*> componentArrays;
	for(uint32_t i = 0; i < systems.size(); i++) {
		const Array<uint32_t>& componentTypes = systems[i]->getComponentTypes();
		if(componentTypes.size() == 1) {
			size_t typeSize = BaseECSComponent::getTypeSize(componentTypes[0]);
			Array<uint8_t>& array = components[componentTypes[0]];
			for(uint32_t j = 0; j < array.size(); j += typeSize) {
				BaseECSComponent* component = (BaseECSComponent*)&array[j];
				systems[i]->updateComponents(delta, &component);
			}
		} else {
			updateSystemWithMultipleComponents(i, systems, delta, componentTypes, componentParam, componentArrays);
		}
	}
}

uint32_t ECS::findLeastCommonComponent(const Array<uint32_t>& componentTypes, const Array<uint32_t>& componentFlags)
{
	uint32_t minSize = (uint32_t)-1;
	uint32_t minIndex = (uint32_t)-1;
	for(uint32_t i = 0; i < componentTypes.size(); i++) {
		if((componentFlags[i] & BaseECSSystem::FLAG_OPTIONAL) != 0) {
			continue;
		}
		size_t typeSize = BaseECSComponent::getTypeSize(componentTypes[i]);
		uint32_t size = components[componentTypes[i]].size()/typeSize;
		if(size <= minSize) {
			minSize = size;
			minIndex = i;
		}
	}

	return minIndex;
}

void ECS::updateSystemWithMultipleComponents(uint32_t index, ECSSystemList& systems, float delta,
		const Array<uint32_t>& componentTypes, Array<BaseECSComponent*>& componentParam,
		Array<Array<uint8_t>*>& componentArrays)
{
	const Array<uint32_t>& componentFlags = systems[index]->getComponentFlags();

	componentParam.resize(Math::max(componentParam.size(), componentTypes.size()));
	componentArrays.resize(Math::max(componentArrays.size(), componentTypes.size()));
	for(uint32_t i = 0; i < componentTypes.size(); i++) {
		componentArrays[i] = &components[componentTypes[i]];
	}
	uint32_t minSizeIndex = findLeastCommonComponent(componentTypes, componentFlags);

	size_t typeSize = BaseECSComponent::getTypeSize(componentTypes[minSizeIndex]);
	Array<uint8_t>& array = *componentArrays[minSizeIndex];
	for(uint32_t i = 0; i < array.size(); i += typeSize) {
		componentParam[minSizeIndex] = (BaseECSComponent*)&array[i];
		Array<std::pair<uint32_t, uint32_t> >& entityComponents =
			handleToEntity(componentParam[minSizeIndex]->entity);

		bool isValid = true;
		for(uint32_t j = 0; j < componentTypes.size(); j++) {
			if(j == minSizeIndex) {
				continue;
			}

			componentParam[j] = getComponentInternal(entityComponents,
					*componentArrays[j], componentTypes[j]);
			if(componentParam[j] == nullptr && (componentFlags[j] & BaseECSSystem::FLAG_OPTIONAL) == 0) {
				isValid = false;
				break;
			}
		}

		if(isValid) {
			systems[index]->updateComponents(delta, &componentParam[0]);
		}
	}
}

