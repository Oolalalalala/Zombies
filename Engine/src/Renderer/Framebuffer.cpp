#include "pch.h"
#include "Framebuffer.h"

#include <iostream>


Ref<Framebuffer> Framebuffer::Create(FramebufferSpecification specification)
{
	return CreateRef<Framebuffer>(specification);
}

Framebuffer::Framebuffer(FramebufferSpecification specification)
	: m_Specification(specification)
{
	Invalidate();
}

Framebuffer::~Framebuffer()
{
	if (!m_RendererID)
		return;

	glDeleteFramebuffers(1, &m_RendererID);
	glDeleteTextures(1, &m_ColorBufferRendererID);
	glDeleteTextures(1, &m_DepthBufferRendererID);
}

void Framebuffer::Invalidate()
{
	if (m_RendererID)
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorBufferRendererID);
		glDeleteTextures(1, &m_DepthBufferRendererID);
	}

	glCreateFramebuffers(1, &m_RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorBufferRendererID);
	glTextureStorage2D(m_ColorBufferRendererID, 1, m_Specification.ColorBufferInternalFormat, m_Specification.Width, m_Specification.Height);
	glTextureParameteri(m_ColorBufferRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_ColorBufferRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_ColorBufferRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_ColorBufferRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthBufferRendererID);
	glTextureStorage2D(m_DepthBufferRendererID, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
	glTextureParameteri(m_DepthBufferRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_DepthBufferRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_DepthBufferRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_DepthBufferRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBufferRendererID, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthBufferRendererID, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		CORE_ERROR("Framebuffer creation incomplete.");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(uint32_t width, uint32_t height)
{
	if (width == 0 || height == 0)
		return;

	m_Specification.Width = width;
	m_Specification.Height = height;

	Invalidate();
}

void Framebuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	glViewport(0, 0, m_Specification.Width, m_Specification.Height);
}

