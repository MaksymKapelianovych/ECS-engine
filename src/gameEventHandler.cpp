#include "gameEventHandler.hpp"
#include "core/input.hpp"

#define MOUSE_OFFSET Input::NUM_KEYS

void GameEventHandler::onKeyDown(uint32_t keyCode, bool isRepeat)
{
	updateInput(keyCode, 1.0f, isRepeat);
}

void GameEventHandler::onKeyUp(uint32_t keyCode, bool isRepeat)
{
	updateInput(keyCode, -1.0f, isRepeat);
}

void GameEventHandler::onMouseDown(uint32_t mouseButton, uint8_t numClicks)
{
	updateInput(mouseButton+MOUSE_OFFSET, 1.0f, false);
}

void GameEventHandler::onMouseUp(uint32_t mouseButton, uint8_t numClicks)
{
	updateInput(mouseButton+MOUSE_OFFSET, -1.0f, false);
}

void GameEventHandler::onMouseMove(uint32_t mousePosX, uint32_t mousePosY, 
		uint32_t deltaX, uint32_t deltaY) {}

void GameEventHandler::addKeyControl(uint32_t keyCode, InputControl& inputControl, float weight)
{
	inputs[keyCode].emplace_back(weight, inputControl);
}
void GameEventHandler::addMouseControl(uint32_t mouseButton, InputControl& inputControl, float weight)
{
	inputs[mouseButton+MOUSE_OFFSET].emplace_back(weight, inputControl);
}

void GameEventHandler::updateInput(uint32_t inputCode, float dir, bool isRepeat)
{
	if(isRepeat) {
		return;
	}

	for (auto& input : inputs[inputCode])
	{
		input.second.addAmt(input.first * dir);
	}
}
