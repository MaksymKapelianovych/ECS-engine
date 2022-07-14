#pragma once

#include "renderDevice.hpp"

class UniformBuffer
{
public:
	inline UniformBuffer(RenderDevice& deviceIn, uintptr_t dataSize,
		enum RenderDevice::BufferUsage usage, const void* data = nullptr) :
		device(&deviceIn), 
		deviceId(device->createUniformBuffer(data, dataSize, usage)),
		size(dataSize) {}
	inline ~UniformBuffer()
	{
		deviceId = device->releaseUniformBuffer(deviceId);
	}

	inline void update(const void* data, uintptr_t dataSize);
	inline void update(const void* data) { update(data, size); }
	
	inline uint32_t getId();
private:
	RenderDevice* device;
	uint32_t deviceId;
	uintptr_t size;

	NULL_COPY_AND_ASSIGN(UniformBuffer);
};

inline uint32_t UniformBuffer::getId()
{
	return deviceId;
}

inline void UniformBuffer::update(const void* data, uintptr_t dataSize)
{
	device->updateUniformBuffer(deviceId, data, dataSize);
}

