#pragma once

#include "renderDevice.hpp"
#include "uniformBuffer.hpp"
#include "texture.hpp"
#include "sampler.hpp"

class Shader
{
public:
	inline Shader(RenderDevice& deviceIn, const String& text) :
		device(&deviceIn),
		deviceId(device->createShaderProgram(text)) {}
	inline ~Shader()
	{
		deviceId = device->releaseShaderProgram(deviceId);
	}

	inline void setUniformBuffer(const String& name, UniformBuffer& buffer);
	inline void setSampler(const String& name, Texture& texture, Sampler& sampler,
			uint32_t unit);
	inline uint32_t getId();
private:
	RenderDevice* device;
	uint32_t deviceId;

	NULL_COPY_AND_ASSIGN(Shader);
};

inline uint32_t Shader::getId()
{
	return deviceId;
}

inline void Shader::setUniformBuffer(const String& name, UniformBuffer& buffer)
{
	device->setShaderUniformBuffer(deviceId, name, buffer.getId());
}

inline void Shader::setSampler(const String& name, Texture& texture, Sampler& sampler,
		uint32_t unit)
{
	device->setShaderSampler(deviceId, name, texture.getId(), sampler.getId(), unit);
}

