#pragma once
#include "core/common.hpp"

class DDSTexture
{
public:
	DDSTexture() : buffer(nullptr) {}
	virtual ~DDSTexture();

	bool load(const char* fileName);
	inline uint32_t getMipMapCount() const {
		return mipMapCount;
	}

	inline uint32_t getFourCC() const {
		return fourCC;
	}

	inline uint32_t getWidth() const {
		return width;
	}

	inline uint32_t getHeight() const {
		return height;
	}

	inline const unsigned char* getBuffer()  const {
		return buffer;
	}
private:
	unsigned char* buffer;
	uint32_t height;
	uint32_t width;
	uint32_t mipMapCount;
	uint32_t fourCC;
	
	void cleanup();
	NULL_COPY_AND_ASSIGN(DDSTexture);
};

