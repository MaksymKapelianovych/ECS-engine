#pragma once

#include "renderDevice.hpp"
#include "texture.hpp"
#include <stdexcept>

class RenderTarget
{
public:
	RenderTarget(RenderDevice& deviceIn) :
		device(&deviceIn),
		deviceId(0) {}

	// TODO: Ensure texture isn't compressed. Otherwise, render target creation fails.
	inline RenderTarget(RenderDevice& deviceIn,
			Texture& texture, uint32_t width, uint32_t height,
			enum RenderDevice::FramebufferAttachment attachment
				= RenderDevice::ATTACHMENT_COLOR,
			uint32_t attachmentNumber = 0, uint32_t mipLevel = 0) :
		device(&deviceIn),
		deviceId(device->createRenderTarget(texture.getId(), width, height,
					attachment, attachmentNumber, mipLevel)) {
			checkCompressed(texture);
		}
	inline RenderTarget(RenderDevice& deviceIn,
			Texture& texture,
			enum RenderDevice::FramebufferAttachment attachment
				= RenderDevice::ATTACHMENT_COLOR,
			uint32_t attachmentNumber = 0, uint32_t mipLevel = 0) :
		device(&deviceIn),
		deviceId(device->createRenderTarget(texture.getId(),
					texture.getWidth(), texture.getHeight(),
					attachment, attachmentNumber, mipLevel)) {
			checkCompressed(texture);
		}

	inline void checkCompressed(const Texture& texture) {
		if(texture.isCompressed()) {
			DEBUG_LOG(LOG_TYPE_RENDERER, LOG_ERROR, "Compressed textures cannot be used as render targets!");
			throw std::invalid_argument("Compressed textures cannot be used as render targets!");
		}
		if(texture.hasMipmaps()) {
			DEBUG_LOG(LOG_TYPE_RENDERER, LOG_WARNING, "Rendering to a texture with mipmaps will NOT render to all mipmap levels! Unexpected results may occur.");
		}
	}

	inline ~RenderTarget()
	{
		deviceId = device->releaseRenderTarget(deviceId);
	}

	inline uint32_t getId();
private:
	RenderDevice* device;
	uint32_t deviceId;

	NULL_COPY_AND_ASSIGN(RenderTarget);
};

inline uint32_t RenderTarget::getId()
{
	return deviceId;
}


