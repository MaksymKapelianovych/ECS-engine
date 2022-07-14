#pragma once

#include "core/common.hpp"
#include "dataStructures/string.hpp"

class ArrayBitmap
{
public:
	ArrayBitmap(int32_t width=1, int32_t height=1);
	ArrayBitmap(int32_t width, int32_t height, int32_t* pixels);
	ArrayBitmap(int32_t width, int32_t height, int32_t* pixels, int32_t offsetX,
			int32_t offsetY, int32_t rowOffset);
	ArrayBitmap(const String& fileName);
	virtual ~ArrayBitmap();

	bool load(const String& fileName);
	bool save(const String& fileName) const;
	void clear(int32_t color);

	inline int32_t getWidth() const;
	inline int32_t getHeight() const;

	inline int32_t get(int32_t x, int32_t y) const;
	inline void set(int32_t x, int32_t y, int32_t pixel);
	inline int32_t* getPixelArray();
	inline const int32_t* getPixelArray() const;
private:
	int32_t width;
	int32_t height;
	int32_t* pixels;

	inline uintptr_t getPixelsSize() const;
	NULL_COPY_AND_ASSIGN(ArrayBitmap)
};

inline int32_t ArrayBitmap::getWidth() const
{
	return width;
}

inline int32_t ArrayBitmap::getHeight() const
{
	return height;
}

inline uintptr_t ArrayBitmap::getPixelsSize() const
{
	return (uintptr_t)(width * height) * sizeof(pixels[0]);
}

inline int32_t ArrayBitmap::get(int32_t x, int32_t y) const
{
	assertCheck(x >= 0 && x < width);
	assertCheck(y >= 0 && y < height);
	return pixels[x + y * width];
}

inline void ArrayBitmap::set(int32_t x, int32_t y, int32_t pixel)
{
	assertCheck(x >= 0 && x < width);
	assertCheck(y >= 0 && y < height);
	pixels[x + y * width] = pixel;
}

inline int32_t* ArrayBitmap::getPixelArray()
{
	return pixels;
}

inline const int32_t* ArrayBitmap::getPixelArray() const
{
	return pixels;
}

