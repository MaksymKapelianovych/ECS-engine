#pragma once

#include "ecs/ecs.hpp"
#include "utilComponents.hpp"
#include "motion.hpp"
#include "math/transform.hpp"

struct MovementControl
{
	Vector3f movement;
	AxisInputControl* inputControl;
	float lastInputAmt;

	MovementControl(const Vector3f& movementIn, AxisInputControl* inputControlIn) :
		movement(movementIn), inputControl(inputControlIn), lastInputAmt(inputControlIn->getAmt()) {}

	friend std::ostream& operator<<(std::ostream& os, const MovementControl& value)
	{
		os << '{';
		os << value.movement << ", ";
		os << value.inputControl << ", ";
		os << value.lastInputAmt;
		os << '}';
		return os;
	}
};

struct MovementControlComponent : public ECSComponent<MovementControlComponent>
{
	Array<MovementControl> movementControls;
};

inline std::ostream& operator<<(std::ostream& os, const MovementControlComponent& value)
{
	os << std::setw(ECS_LOG_MARGIN) << StringFuncs::toString(value.movementControls);
	return os;
}

ECS_COMPONENT_PRINT(MovementControlComponent, "movementControls");


class MovementControlSystem : public BaseECSSystem
{
public:
	MovementControlSystem(ECS& ecsIn) : BaseECSSystem(ecsIn)
	{
		addComponentType(MovementControlComponent::ID);
		addComponentType(MotionComponent::ID);
	}

	virtual void updateComponents(float delta, BaseECSComponent** components)
	{
		MovementControlComponent* movementControl = (MovementControlComponent*)components[0];
		MotionComponent* motionComponent = (MotionComponent*)components[1];

		// std::cout << "MovementControlSystem   " << ecs.handleToEntityIndex(movementControl->entity) << "   " << ecs.handleToEntityIndex(motionComponent->entity) << std::endl;

		
		// for (auto& control : movementControl->movementControls)
		// {
		// 	Vector3f movement = control.movement;
		// 	float    inputAmt = control.inputControl->getAmt() / 10;
		// 	float    inputDelta = inputAmt - control.lastInputAmt;
		// 	motionComponent->acceleration += movement * inputDelta;
		// 	control.lastInputAmt = inputAmt;
		// }

		for (auto& control : movementControl->movementControls)
		{
			Vector3f movement = control.movement;
			float    inputAmt = control.inputControl->getAmt();
			motionComponent->acceleration += movement * inputAmt * delta;
		}
	}
};
