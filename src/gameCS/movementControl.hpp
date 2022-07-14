#pragma once

#include "ecs/ecs.hpp"
#include "utilComponents.hpp"
#include "motion.hpp"
#include "math/transform.hpp"

struct MovementControl
{
	Vector3f movement;
	InputControl* inputControl;
	float lastInputAmt;

	MovementControl(const Vector3f& movementIn, InputControl* inputControlIn) :
		movement(movementIn), inputControl(inputControlIn), lastInputAmt(inputControlIn->getAmt()) {}
};

struct MovementControlComponent : public ECSComponent<MovementControlComponent>
{
	Array<MovementControl> movementControls;
};

class MovementControlSystem : public BaseECSSystem
{
public:
	MovementControlSystem() : BaseECSSystem()
	{
		addComponentType(MovementControlComponent::ID);
		addComponentType(MotionComponent::ID);
	}

	virtual void updateComponents(float delta, BaseECSComponent** components)
	{
		MovementControlComponent* movementControl = (MovementControlComponent*)components[0];
		MotionComponent* motionComponent = (MotionComponent*)components[1];
		for (auto& control : movementControl->movementControls)
		{
			Vector3f movement = control.movement;
			float    inputAmt = control.inputControl->getAmt();
			float    inputDelta = inputAmt - control.lastInputAmt;
			motionComponent->acceleration += movement * inputDelta;
			control.lastInputAmt = inputAmt;
		}
		// TODO: This is a cheap hack to get resistance for now! Replace with a proper system later
		motionComponent->velocity = motionComponent->velocity * 0.95f;
	}
};
