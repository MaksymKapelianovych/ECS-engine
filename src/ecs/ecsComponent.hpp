#pragma once

#include "core/common.hpp"
#include "dataStructures/array.hpp"
#include <tuple>
#include <iostream>
#include <iomanip>
#include "string.hpp"

struct BaseECSComponent;
typedef void* EntityHandle;
// Create new component as copy of BaseECSComponent* copyFrom
// typedef uint32_t (*ECSComponentCreateFunction)(Array<uint8_t>& memory, EntityHandle entity, BaseECSComponent* copyFrom);
// typedef void (*ECSComponentFreeFunction)(BaseECSComponent* comp);

using ECSComponentCreateFunction = uint32_t (*) (Array<uint8_t>& memory, EntityHandle entity, BaseECSComponent* copyFrom);
using ECSComponentFreeFunction = void (*)(BaseECSComponent* comp);

#if DEBUG

using ECSComponentPrintFunction = void (*)(const uint8_t* comp, uint32_t count);
using ComponentTypesArray = Array<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t, ECSComponentPrintFunction> >;

#else

using ComponentTypesArray = Array<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t> >;

#endif


#define NULL_ENTITY_HANDLE nullptr

struct BaseECSComponent
{
public:
#ifdef DEBUG
	static uint32_t registerComponentType(ECSComponentCreateFunction createfn,
				ECSComponentFreeFunction freefn, size_t size, String name, ECSComponentPrintFunction printfn);
#else
	static uint32_t registerComponentType(ECSComponentCreateFunction createfn,
			ECSComponentFreeFunction freefn, size_t size);
#endif
	
	EntityHandle entity = NULL_ENTITY_HANDLE;

	inline static ECSComponentCreateFunction getTypeCreateFunction(uint32_t id)
	{
		return std::get<0>((*componentTypes)[id]);
	}

	inline static ECSComponentFreeFunction getTypeFreeFunction(uint32_t id)
	{
		return std::get<1>((*componentTypes)[id]);
	}

	inline static size_t getTypeSize(uint32_t id)
	{
		return std::get<2>((*componentTypes)[id]);
	}

	inline static ECSComponentPrintFunction getTypePrintFunction(uint32_t id)
	{
		return std::get<3>((*componentTypes)[id]);
	}

#ifdef DEBUG
	inline static String getComponentName(uint32_t id)
	{
		return (*componentNames)[id];
	}
#endif

	inline static bool isTypeValid(uint32_t id)
	{
		return id < componentTypes->size();
	}
private:
	// std::tuple<CreateFunction, FreeFunction, TypeSize>
	static ComponentTypesArray* componentTypes;
#ifdef DEBUG
	static Array<String>* componentNames;
#endif
	
};

template<typename T>
struct ECSComponent : public BaseECSComponent
{
	// static const ECSComponentCreateFunction CREATE_FUNCTION;
	// static const ECSComponentFreeFunction FREE_FUNCTION;
	static const uint32_t ID;
	static const size_t SIZE; 
};

template<typename Component>
uint32_t ECSComponentCreate(Array<uint8_t>& memory, EntityHandle entity, BaseECSComponent* copyFrom)
{
	uint32_t index = memory.size();
	memory.resize(index+Component::SIZE);
	Component* component = new(&memory[index])Component(*(Component*)copyFrom);
	component->entity = entity;
	return index;
}

template<typename Component>
void ECSComponentFree(BaseECSComponent* comp)
{
	Component* component = (Component*)comp;
	component->~Component();
}

template<typename Component>
void ECSComponentPrint(const uint8_t* comp, uint32_t count)
{
	std::cout << "Not implemented \n";
}

// template<typename Component>
// void ECSComponentPrint(const uint8_t* comp, uint32_t count, Array<String> fields)
// {
// 	for(auto& field : fields)
// 	{
// 		std::cout << std::setw(15) << field;
// 	}
// 	for(uint32_t i = 0; i < count; i++)
// 	{
// 		Component* value = (Component*)(comp + (i * sizeof(Component)));
// 		fprintf(stderr, "%3d %s\n", i, StringFuncs::toString(value).c_str());
// 	}
// }

#ifdef DEBUG
	template<typename T>
	const uint32_t ECSComponent<T>::ID(BaseECSComponent::registerComponentType(ECSComponentCreate<T>, ECSComponentFree<T>, sizeof(T), typeid(T).name(), ECSComponentPrint<T>));
#else
	template<typename T>
	const uint32_t ECSComponent<T>::ID(BaseECSComponent::registerComponentType(ECSComponentCreate<T>, ECSComponentFree<T>, sizeof(T)));
#endif

template<typename T>
const size_t ECSComponent<T>::SIZE(sizeof(T));

// template<typename T>
// const ECSComponentCreateFunction ECSComponent<T>::CREATE_FUNCTION(ECSComponentCreate<T>);
//
// template<typename T>
// const ECSComponentFreeFunction ECSComponent<T>::FREE_FUNCTION(ECSComponentFree<T>);

//BEGIN EXAMPLE CODE
struct TestComponent : public ECSComponent<TestComponent>
{
	float x;
	float y;
};

