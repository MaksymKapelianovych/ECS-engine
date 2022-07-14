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

void GameEventHandler::onMouseMove(int32_t mousePosX, int32_t mousePosY, 
		int32_t deltaX, int32_t deltaY) {}

void GameEventHandler::addKeyControl(uint32_t keyCode, InputControl& inputControl, float weight)
{
	inputs[keyCode].push_back(std::pair<float, InputControl&>(weight, inputControl));
}
void GameEventHandler::addMouseControl(uint32_t mouseButton, InputControl& inputControl, float weight)
{
	inputs[mouseButton+MOUSE_OFFSET].push_back(std::pair<float, InputControl&>(weight, inputControl));
}

void GameEventHandler::updateInput(uint32_t inputCode, float dir, bool isRepeat)
{
	if(isRepeat) {
		return;
	}

	for(uint32_t i = 0; i < inputs[inputCode].size(); i++) {
		inputs[inputCode][i].second.addAmt(inputs[inputCode][i].first * dir);
	}
}
