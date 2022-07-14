#pragma once

#include "ecsComponent.hpp"
#include "ecsSystem.hpp"
#include "dataStructures/map.hpp"

class ECSListener
{
public:
	virtual ~ECSListener() = default;
	
	virtual void onMakeEntity(EntityHandle handle) {}
	virtual void onRemoveEntity(EntityHandle handle) {}
	virtual void onAddComponent(EntityHandle handle, uint32_t id) {}
	virtual void onRemoveComponent(EntityHandle handle, uint32_t id) {}

	const Array<uint32_t>& getComponentIDs() { 
		return componentIDs;
	}

	inline bool shouldNotifyOnAllComponentOperations() const {
		return notifyOnAllComponentOperations;
	}
	inline bool shouldNotifyOnAllEntityOperations() const {
		return notifyOnAllEntityOperations;
	}

protected:
	void setNotificationSettings(
			bool shouldNotifyOnAllComponentOperations,
			bool shouldNotifyOnAllEntityOperations) {
		notifyOnAllComponentOperations = shouldNotifyOnAllComponentOperations;
		notifyOnAllEntityOperations = shouldNotifyOnAllEntityOperations;
	}
	void addComponentID(uint32_t id) {
		componentIDs.push_back(id);
	}
private:
	Array<uint32_t> componentIDs;
	bool notifyOnAllComponentOperations = false;
	bool notifyOnAllEntityOperations = false;
};

class ECS
{
public:
	ECS() = default;
	~ECS();

	// ECSListener methods
	inline void addListener(ECSListener* listener) {
		listeners.push_back(listener);
	}

	// Entity methods
	EntityHandle makeEntity(BaseECSComponent* const* components, uint32_t const* componentIDs, size_t numComponents);
	FORCEINLINE EntityHandle makeEntity(std::initializer_list<BaseECSComponent*> componentsIn, std::initializer_list<const uint32_t> componentIDs, size_t numComponents)
	{
		return makeEntity(componentsIn.begin(), componentIDs.begin(), numComponents);
	}
	void removeEntity(EntityHandle handle);
	

	template <class ...ComponentTypes>
	FORCEINLINE EntityHandle makeEntity(ComponentTypes... componentTypes)
	{
		return makeEntity({(&componentTypes)...}, {ComponentTypes::ID...}, sizeof...(componentTypes));
	}



	// Component methods
	template<class Component>
	inline void addComponent(EntityHandle entity, Component* component)
	{
		addComponentInternal(entity, handleToEntity(entity), Component::ID, component);
		for(uint32_t i = 0; i < listeners.size(); i++) {
			const Array<uint32_t>& componentIDs = listeners[i]->getComponentIDs();
			if(listeners[i]->shouldNotifyOnAllComponentOperations()) {
				listeners[i]->onAddComponent(entity, Component::ID);
			} else {
				for(uint32_t j = 0; j < componentIDs.size(); j++) {
					if(componentIDs[j] == Component::ID) {
						listeners[i]->onAddComponent(entity, Component::ID);
						break;
					}
				}
			}
		}
	}

	template<class Component>
	bool removeComponent(EntityHandle entity)
	{
		for(uint32_t i = 0; i < listeners.size(); i++) {
			const Array<uint32_t>& componentIDs = listeners[i]->getComponentIDs();
			for(uint32_t j = 0; j < componentIDs.size(); j++) {
				if(listeners[i]->shouldNotifyOnAllComponentOperations()) {
					listeners[i]->onRemoveComponent(entity, Component::ID);
				} else {
					if(componentIDs[j] == Component::ID) {
						listeners[i]->onRemoveComponent(entity, Component::ID);
						break;
					}
				}
			}
		}
		return removeComponentInternal(entity, Component::ID);
	}

	template<class Component>
	Component* getComponent(EntityHandle entity)
	{
		return (Component*)getComponentInternal(handleToEntity(entity), components[Component::ID], Component::ID);
	}

	BaseECSComponent* getComponentByType(EntityHandle entity, uint32_t componentID)
	{
		return getComponentInternal(handleToEntity(entity), components[componentID], componentID);
	}

	// System methods
	void updateSystems(ECSSystemList& systems, float delta);
	
private:
	Map<uint32_t, Array<uint8_t>> components;
	Array<std::pair<uint32_t, Array<std::pair<uint32_t, uint32_t> > >* > entities;
	Array<ECSListener*> listeners;

	inline std::pair<uint32_t, Array<std::pair<uint32_t, uint32_t> > >* handleToRawType(EntityHandle handle)
	{
		return (std::pair<uint32_t, Array<std::pair<uint32_t, uint32_t> > >*)handle;
	}

	inline uint32_t handleToEntityIndex(EntityHandle handle)
	{
		return handleToRawType(handle)->first;
	}

	inline Array<std::pair<uint32_t, uint32_t> >& handleToEntity(EntityHandle handle)
	{
		return handleToRawType(handle)->second;
	}

	void deleteComponent(uint32_t componentID, uint32_t index);
	bool removeComponentInternal(EntityHandle handle, uint32_t componentID);
	void addComponentInternal(EntityHandle handle, Array<std::pair<uint32_t, uint32_t> >& entity, uint32_t componentID, BaseECSComponent* component);
	BaseECSComponent* getComponentInternal(Array<std::pair<uint32_t, uint32_t> >& entityComponents, Array<uint8_t>& array, uint32_t componentID);

	void updateSystemWithMultipleComponents(uint32_t index, ECSSystemList& systems, float delta, const Array<uint32_t>& componentTypes,
			Array<BaseECSComponent*>& componentParam, Array<Array<uint8_t>*>& componentArrays);
	uint32_t findLeastCommonComponent(const Array<uint32_t>& componentTypes, const Array<uint32_t>& componentFlags);

	NULL_COPY_AND_ASSIGN(ECS);
};
