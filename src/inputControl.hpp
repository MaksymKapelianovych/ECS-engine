#pragma once

#include "math/math.hpp"

class InputControl
{
public:
	InputControl();
	void addAmt(float amtToAdd);
	float getAmt() const;
private:
	float amt;
};

inline InputControl::InputControl() :
	amt(0.0f) {}

inline void InputControl::addAmt(float amtToAdd)
{
	amt = Math::clamp(amt + amtToAdd, -1.f, 1.0f);
}

inline float InputControl::getAmt() const
{
	return amt;
}
