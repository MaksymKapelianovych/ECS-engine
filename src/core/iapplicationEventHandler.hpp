#pragma once

#include "common.hpp"

class IApplicationEventHandler
{
public:
	IApplicationEventHandler() = default;
	virtual ~IApplicationEventHandler() = default;
	virtual void onKeyDown(uint32_t keyCode, bool isRepeat) = 0;
	virtual void onKeyUp(uint32_t keyCode, bool isRepeat) = 0;
	virtual void onMouseDown(uint32_t mouseButton, uint8_t numClicks) = 0 ;
	virtual void onMouseUp(uint32_t mouseButton, uint8_t numClicks) = 0;
	virtual void onMouseMove(uint32_t mousePosX, uint32_t mousePosY, 
			uint32_t deltaX, uint32_t deltaY) = 0;
private:
	NULL_COPY_AND_ASSIGN(IApplicationEventHandler);
};
