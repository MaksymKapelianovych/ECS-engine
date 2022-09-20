#pragma once

#include "core/iapplicationEventHandler.hpp"
#include "dataStructures/map.hpp"
#include "dataStructures/array.hpp"
#include "AxisInputControl.hpp"

class GameEventHandler : public IApplicationEventHandler
{
public:
	GameEventHandler() = default;
	virtual ~GameEventHandler() = default;
	virtual void onKeyDown(uint32_t keyCode, bool isRepeat);
	virtual void onKeyUp(uint32_t keyCode, bool isRepeat);
	virtual void onMouseDown(uint32_t mouseButton, uint8_t numClicks);
	virtual void onMouseUp(uint32_t mouseButton, uint8_t numClicks);
	virtual void onMouseMove(uint32_t mousePosX, uint32_t mousePosY, 
			uint32_t deltaX, uint32_t deltaY);

	void addKeyControl(uint32_t keyCode, AxisInputControl& inputControl, float weight = 1.0f);
	void addMouseControl(uint32_t mouseButton, AxisInputControl& inputControl, float weight = 1.0f);
	void addKeyControl(uint32_t keyCode, ActionInputControl& inputControl, bool pressed = true);
	void addMouseControl(uint32_t mouseButton, ActionInputControl& inputControl, bool pressed = true);
private:
	Map<uint32_t, Array<std::pair<float, AxisInputControl&> > > axisInputs;
	Map<uint32_t, Array<std::pair<bool, ActionInputControl&>>> actionInputs;
	void updateAxisInput(uint32_t inputCode, float dir, bool isRepeat);
	void updateActionInput(uint32_t inputCode, bool pressed);
	NULL_COPY_AND_ASSIGN(GameEventHandler);
};
