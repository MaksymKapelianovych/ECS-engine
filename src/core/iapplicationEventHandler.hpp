#pragma once

#include "common.hpp"

class IApplicationEventHandler
{
public:
	IApplicationEventHandler() {}
	virtual ~IApplicationEventHandler() {}
	virtual void onKeyDown(uint32_t keyCode, bool isRepeat) {}
	virtual void onKeyUp(uint32_t keyCode, bool isRepeat) {}
	virtual void onMouseDown(uint32_t mouseButton, uint8_t numClicks) {}
	virtual void onMouseUp(uint32_t mouseButton, uint8_t numClicks) {}
	virtual void onMouseMove(uint32_t mousePosX, uint32_t mousePosY, 
			uint32_t deltaX, uint32_t deltaY) {}
private:
	NULL_COPY_AND_ASSIGN(IApplicationEventHandler);
};
