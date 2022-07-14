#pragma once

#include "renderDevice.hpp"
#include "indexedModel.hpp"

class VertexArray
{
public:
	inline VertexArray(RenderDevice& deviceIn, const IndexedModel& model,
			enum RenderDevice::BufferUsage usage) :
		device(&deviceIn),
		deviceId(model.createVertexArray(deviceIn, usage)),
		numIndices(model.getNumIndices()) {}
	inline ~VertexArray()
	{
		deviceId = device->releaseVertexArray(deviceId);
	}

	inline void updateBuffer(uint32_t bufferIndex, const void* data, uintptr_t dataSize);

	inline uint32_t getId();
	inline uint32_t getNumIndices();
private:
	RenderDevice* device;
	uint32_t deviceId;
	uint32_t numIndices;

	NULL_COPY_AND_ASSIGN(VertexArray);
};

inline uint32_t VertexArray::getId()
{
	return deviceId;
}

inline uint32_t VertexArray::getNumIndices()
{
	return numIndices;
}

inline void VertexArray::updateBuffer(uint32_t bufferIndex,
		const void* data, uintptr_t dataSize)
{
	return device->updateVertexArrayBuffer(deviceId, bufferIndex, data, dataSize);
}

