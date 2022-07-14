#include "arrayBitmap.hpp"
#include "core/memory.hpp"
#include "staticLibs/stb_image.h"

ArrayBitmap::ArrayBitmap(int32_t widthIn, int32_t heightIn) :
	width(widthIn), height(heightIn)
{
	assertCheck(width > 0 && height > 0);
	pixels = (int32_t*)Memory::malloc(getPixelsSize());
}

ArrayBitmap::ArrayBitmap(int32_t widthIn, int32_t heightIn, int32_t* pixelsIn) :
	width(widthIn), height(heightIn)
{
	assertCheck(width > 0 && height > 0);
	assertCheck(pixelsIn != nullptr);
	uintptr_t size = getPixelsSize();
	pixels = (int32_t*)Memory::malloc(size);
	Memory::memcpy(pixels, pixelsIn, size);
}

ArrayBitmap::ArrayBitmap(int32_t widthIn, int32_t heightIn, int32_t* pixelsIn, int32_t offsetX,
		int32_t offsetY, int32_t rowOffset) :
	width(widthIn), height(heightIn)
{
	assertCheck(width > 0 && height > 0);
	assertCheck(pixelsIn != nullptr);
	assertCheck(offsetX > 0 && offsetY > 0 && rowOffset > 0);
	uintptr_t size = getPixelsSize();
	pixels = (int32_t*)Memory::malloc(size);
	int32_t* pixelsSrc = pixelsIn + offsetY + offsetX * rowOffset;

	for(uintptr_t i = 0; i < (uintptr_t)height;
			++i, pixels += width, pixelsSrc += rowOffset) {
		Memory::memcpy(pixels, pixelsSrc, (uintptr_t)width);
	}
}

ArrayBitmap::~ArrayBitmap()
{
	pixels = (int32_t*)Memory::free(pixels);
}

bool ArrayBitmap::load(const String& fileName)
{
	int32_t texWidth, texHeight, bytesPerPixel;
	uint8_t* data = stbi_load(fileName.c_str(), &texWidth, &texHeight,
			&bytesPerPixel, 4);
	if(data == nullptr) {
		return false;
	}

	if(texWidth == width && texHeight == height) {
		Memory::memcpy(pixels, data, getPixelsSize());
	} else {
		width = texWidth;
		height = texHeight;
		pixels = (int32_t*)Memory::free(pixels);
		pixels = (int32_t*)Memory::malloc(getPixelsSize());
		Memory::memcpy(pixels, data, getPixelsSize());
	}

	stbi_image_free(data);
	return true;
}

bool ArrayBitmap::save(const String& fileName) const
{
	// TODO: Implement
	return false;
}

void ArrayBitmap::clear(int32_t color)
{
	Memory::memset(pixels, color, getPixelsSize());
}


