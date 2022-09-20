#include "ecsComponent.hpp"

ComponentTypesArray* BaseECSComponent::componentTypes;

#ifdef DEBUG
Array<String>* BaseECSComponent::componentNames;

uint32_t BaseECSComponent::registerComponentType(ECSComponentCreateFunction createfn,
			ECSComponentFreeFunction freefn, size_t size, String name, ECSComponentPrintFunction printfn)
{
	if(componentTypes == nullptr) {
		componentTypes = new ComponentTypesArray();
	}
	if(componentNames == nullptr) {
		componentNames = new Array<String>();
	}
	
	const uint32_t componentID = componentTypes->size();
	componentTypes->emplace_back(createfn, freefn, size, printfn);
	componentNames->push_back(name);
	return componentID;
}

#else

uint32_t BaseECSComponent::registerComponentType(ECSComponentCreateFunction createfn,
			ECSComponentFreeFunction freefn, size_t size)
{
	if(componentTypes == nullptr) {
		componentTypes = new ComponentTypesArray();
	}
	if(componentNames == nullptr) {
		componentNames = new Array<String>();
	}
	
	uint32_t componentID = componentTypes->size();
	componentTypes->emplace_back(createfn, freefn, size));
	return componentID;
}

#endif