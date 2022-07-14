#pragma once

#include "core/iapplicationEventHandler.hpp"
#include "dataStructures/map.hpp"
#include "dataStructures/array.hpp"
#include "inputControl.hpp"

class GameEventHandler : public IApplicationEventHandler
{
public:
	GameEventHandler() {}
	virtual ~GameEventHandler() {}
	virtual void onKeyDown(uint32_t keyCode, bool isRepeat);
	virtual void onKeyUp(uint32_t keyCode, bool isRepeat);
	virtual void onMouseDown(uint32_t mouseButton, uint8_t numClicks);
	virtual void onMouseUp(uint32_t mouseButton, uint8_t numClicks);
	virtual void onMouseMove(int32_t mousePosX, int32_t mousePosY, 
			int32_t deltaX, int32_t deltaY);

	void addKeyControl(uint32_t keyCode, InputControl& inputControl, float weight = 1.0f);
	void addMouseControl(uint32_t mouseButton, InputControl& inputControl, float weight = 1.0f);
private:
	Map<uint32_t, Array<std::pair<float, InputControl&> > > inputs;
	void updateInput(uint32_t inputCode, float dir, bool isRepeat);
	NULL_COPY_AND_ASSIGN(GameEventHandler);
};
