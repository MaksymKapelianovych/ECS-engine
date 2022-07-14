#include "ddstexture.hpp"
#include "core/memory.hpp"

DDSTexture::~DDSTexture() {
	cleanup();
}

bool DDSTexture::load(const char* fileName) {
	unsigned char header[124];
	FILE* fp = fopen(fileName, "rb");
	if (fp == NULL) {
		return false;
	}

	// Verify valid DDS file
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return false;
	}

	// Read header
	fread(&header, ARRAY_SIZE_IN_ELEMENTS(header), 1, fp); 
	height      = *(uint32_t*)&(header[8 ]);
	width       = *(uint32_t*)&(header[12]);
	uint32_t linearSize  = *(uint32_t*)&(header[16]);
	mipMapCount = *(uint32_t*)&(header[24]);
	fourCC      = *(uint32_t*)&(header[80]);

	// Allocate memory for DDS file
	uint32_t bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	cleanup();
	buffer = (unsigned char*)Memory::malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	fclose(fp);
	
	return true;
}

void DDSTexture::cleanup() {
	if(buffer != nullptr) {
		Memory::free(buffer);
	}
	buffer = nullptr;

}
