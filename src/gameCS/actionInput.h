#pragma once

#include <functional>

#include "axisInputControl.hpp"
#include "log_utils.h"
#include "ecs/ecs.hpp"

struct ActionInput
{
    ActionInputControl* inputControl;
    std::function<void()> function;

    ActionInput(ActionInputControl* inputControlIn, const std::function<void()>& functionIn)
        : inputControl(inputControlIn), function(functionIn) {}

    friend std::ostream& operator<<(std::ostream& os, const ActionInput& value)
    {
        os << '{';
        os << value.inputControl << ", ";
        os << "some function";
        os << '}';
        return os;
    }
};

struct ActionInputComponent : public ECSComponent<ActionInputComponent>
{
    Array<ActionInput> inputs;
};

inline std::ostream& operator<<(std::ostream& os, const ActionInputComponent& value)
{
	os << std::setw(ECS_LOG_MARGIN) << StringFuncs::toString(value.inputs);
    return os;
}

ECS_COMPONENT_PRINT(ActionInputComponent, "inputControl", "function");

class ActionInputSystem : public BaseECSSystem
{
public:
    ActionInputSystem(ECS& ecsIn) : BaseECSSystem(ecsIn)
    {
        addComponentType(ActionInputComponent::ID);
    }

    virtual void updateComponents(float delta, BaseECSComponent** components) override
    {
        ActionInputComponent* inputComponent = (ActionInputComponent*)components[0];
        for (auto& input : inputComponent->inputs)
        {
            if(input.inputControl->isActive())
            {
                input.function();
                input.inputControl->deactivate();
            }
        }
    }
};
