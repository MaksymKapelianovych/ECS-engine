#pragma once


#include "math/math.hpp"

class AxisInputControl
{
public:
	AxisInputControl();
	void addAmt(float amtToAdd);
	float getAmt() const;
private:
	float amt;
};

inline AxisInputControl::AxisInputControl() :
	amt(0.0f) {}

inline void AxisInputControl::addAmt(float amtToAdd)
{
	amt = Math::clamp(amt + amtToAdd, -1.f, 1.0f);
}

inline float AxisInputControl::getAmt() const
{
	return amt;
}

class ActionInputControl
{

public:
	ActionInputControl(bool isActiveIn);
	void deactivate();
	void activate();
	bool isActive() const;
private:
	bool active;

};

inline ActionInputControl::ActionInputControl(bool activeIn = false) :
	active(activeIn) {}

inline void ActionInputControl::deactivate()
{
	active = false;
}

inline void ActionInputControl::activate()
{
	active = true;
}

inline bool ActionInputControl::isActive() const
{
	return active;
}


