#pragma once

#include "ecsComponent.hpp"

class ECS;

class BaseECSSystem
{
public:
	enum
	{
		FLAG_OPTIONAL = 1,
	};
	BaseECSSystem(ECS& ecsIn) : ecs(ecsIn){};
	virtual ~BaseECSSystem() = default;
	virtual void updateComponents(float delta, BaseECSComponent** components) = 0;
	const Array<uint32_t>& getComponentTypes()
	{
		return componentTypes;
	}
	const Array<uint32_t>& getComponentFlags()
	{
		return componentFlags;
	}
	bool isValid();
protected:
	void addComponentType(uint32_t componentType, uint32_t componentFlag = 0)
	{
		componentTypes.push_back(componentType);
		componentFlags.push_back(componentFlag);
	}

	ECS& ecs;
private:
	Array<uint32_t> componentTypes;
	Array<uint32_t> componentFlags;
};

class ECSSystemList
{
public:
	inline bool addSystem(BaseECSSystem& system)
	{
		if(!system.isValid()) {
			return false;
		}
		systems.push_back(&system);
		return true;
	}
	inline size_t size() const {
		return systems.size();
	}
	inline BaseECSSystem* operator[](uint32_t index) const {
		return systems[index];
	}
	bool removeSystem(BaseECSSystem& system);
private:
	Array<BaseECSSystem*> systems;
};
