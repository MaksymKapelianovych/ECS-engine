#pragma once

#include "renderDevice.hpp"

class Sampler
{
public:
	inline Sampler(RenderDevice& deviceIn,
			enum RenderDevice::SamplerFilter minFilter
				= RenderDevice::FILTER_NEAREST_MIPMAP_LINEAR,
			enum RenderDevice::SamplerFilter magFilter = RenderDevice::FILTER_LINEAR,
			enum RenderDevice::SamplerWrapMode wrapU = RenderDevice::WRAP_CLAMP,
			enum RenderDevice::SamplerWrapMode wrapV = RenderDevice::WRAP_CLAMP,
			float anisotropy = 0.0f) :
		device(&deviceIn),
		deviceId(device->createSampler(minFilter, magFilter,
				wrapU, wrapV, anisotropy)) {}
	inline ~Sampler()
	{
		deviceId = device->releaseSampler(deviceId);
	}

	inline uint32_t getId();
private:
	RenderDevice* device;
	uint32_t deviceId;

	NULL_COPY_AND_ASSIGN(Sampler);
};

inline uint32_t Sampler::getId()
{
	return deviceId;
}

