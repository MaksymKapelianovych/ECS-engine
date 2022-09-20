#include "gameEventHandler.hpp"
#include "core/input.hpp"

#define MOUSE_OFFSET Input::NUM_KEYS

void GameEventHandler::onKeyDown(uint32_t keyCode, bool isRepeat)
{
	updateAxisInput(keyCode, 1.0f, isRepeat);
	updateActionInput(keyCode, true);
}

void GameEventHandler::onKeyUp(uint32_t keyCode, bool isRepeat)
{
	updateAxisInput(keyCode, -1.0f, isRepeat);
	updateActionInput(keyCode, false);
}

void GameEventHandler::onMouseDown(uint32_t mouseButton, uint8_t numClicks)
{
	updateAxisInput(mouseButton+MOUSE_OFFSET, 1.0f, false);
	updateActionInput(mouseButton, true);
}

void GameEventHandler::onMouseUp(uint32_t mouseButton, uint8_t numClicks)
{
	updateAxisInput(mouseButton+MOUSE_OFFSET, -1.0f, false);
	updateActionInput(mouseButton, false);
}

void GameEventHandler::onMouseMove(uint32_t mousePosX, uint32_t mousePosY, 
		uint32_t deltaX, uint32_t deltaY) {}

void GameEventHandler::addKeyControl(uint32_t keyCode, AxisInputControl& inputControl, float weight)
{
	axisInputs[keyCode].emplace_back(weight, inputControl);
}
void GameEventHandler::addMouseControl(uint32_t mouseButton, AxisInputControl& inputControl, float weight)
{
	axisInputs[mouseButton+MOUSE_OFFSET].emplace_back(weight, inputControl);
}

void GameEventHandler::addKeyControl(uint32_t keyCode, ActionInputControl& inputControl, bool pressed)
{
	actionInputs[keyCode].emplace_back(pressed, inputControl);
}

void GameEventHandler::addMouseControl(uint32_t mouseButton, ActionInputControl& inputControl, bool pressed)
{
	actionInputs[mouseButton + MOUSE_OFFSET].emplace_back(pressed, inputControl);
}

void GameEventHandler::updateAxisInput(uint32_t inputCode, float dir, bool isRepeat)
{
	if(isRepeat) {
		return;
	}

	for (auto& input : axisInputs[inputCode])
	{
		input.second.addAmt(input.first * dir);
	}
}

void GameEventHandler::updateActionInput(uint32_t inputCode, bool pressed)
{
	for (auto& input : actionInputs[inputCode])
	{
		if(input.first == pressed)
		{
			input.second.activate();
		}
	}
}
