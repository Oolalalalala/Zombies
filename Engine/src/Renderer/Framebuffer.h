#pragma once

#include <GL/glew.h>
#include "Core/Base.h"


struct FramebufferSpecification
{
	uint32_t Width = 960, Height = 540;
	GLenum ColorBufferInternalFormat = GL_RGBA8, ColorBufferDataFormat = GL_RGBA;
};


class Framebuffer
{
public:
	Framebuffer(FramebufferSpecification specification);
	~Framebuffer();

	void Bind() const;

	void Invalidate();
	void Resize(uint32_t width, uint32_t height);

	const FramebufferSpecification& GetSpecification() { return m_Specification; }

	uint32_t GetColorBufferRendererId() const { return m_ColorBufferRendererID; }

	static Ref<Framebuffer> Create(FramebufferSpecification specification);

private:
	uint32_t m_RendererID = 0;
	uint32_t m_ColorBufferRendererID;
	uint32_t m_DepthBufferRendererID;
	FramebufferSpecification m_Specification;

	friend class RendererAPI;
};
