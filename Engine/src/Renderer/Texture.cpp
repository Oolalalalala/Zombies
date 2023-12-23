#include "pch.h"
#include "Texture.h"

#include "stb_image/stb_image.h"

static GLenum ImageFormatToGLDataFormat(ImageFormat format)
{
	switch (format)
	{
		case ImageFormat::R8:    return GL_RED;
		case ImageFormat::RGB8:  return GL_RGB;
		case ImageFormat::RGBA8: return GL_RGBA;
	}

	CORE_ERROR("Unknown image format");
	return 0;
}

static GLenum ImageFormatToGLInternalFormat(ImageFormat format)
{
	switch (format)
	{
		case ImageFormat::R8:    return GL_R8;
		case ImageFormat::RGB8:  return GL_RGB8;
		case ImageFormat::RGBA8: return GL_RGBA8;
	}

	CORE_ERROR("Unknown image format");
	return 0;
}

static GLenum TextureWrapFormatToGLWrapFormat(TextureWrapFormat format)
{
	switch (format)
	{
		case TextureWrapFormat::Repeat: return GL_REPEAT;
		case TextureWrapFormat::ClampToEdge: return GL_CLAMP_TO_EDGE;
		case TextureWrapFormat::ClampToBorder: return GL_CLAMP_TO_BORDER;
	}

	CORE_ERROR("Unknown texture wrap format");
	return 0;
}

static GLenum TextureFilterFormatToGLFilterFormat(TextureFilterFormat format)
{
	switch (format)
	{
		case TextureFilterFormat::Linear: return GL_LINEAR;
		case TextureFilterFormat::Nearest: return GL_NEAREST;
	}

	CORE_ERROR("Unknown texture filter format");
	return 0;
}



Ref<Texture2D> Texture2D::Create(const std::string& path, const Texture2DSpecification& specification)
{
	return CreateRef<Texture2D>(path, specification);
}

Ref<Texture2D> Texture2D::Create(const Texture2DSpecification& specification)
{
	return CreateRef<Texture2D>(specification);
}

Texture2D::Texture2D(const std::string& path, const Texture2DSpecification& specification)
	: m_MipmapLevel(specification.MipmapLevel)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);

	auto* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

	if (!data)
	{
		CORE_ERROR("Failed to load Texture2D, path = \"{0}\"", path);
		return;
	}

	if (channels == 4)
	{
		m_InternalFormat = specification.SRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
		m_DataFormat = GL_RGBA;
	}
	else if (channels == 3)
	{
		m_InternalFormat = specification.SRGB ? GL_SRGB8 : GL_RGB8;
		m_DataFormat = GL_RGB;
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, m_InternalFormat, width, height);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, TextureFilterFormatToGLFilterFormat(specification.MinFilter));
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, TextureFilterFormatToGLFilterFormat(specification.MagFilter));

	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, TextureWrapFormatToGLWrapFormat(specification.WrapFormat));
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, TextureWrapFormatToGLWrapFormat(specification.WrapFormat));

	glTextureSubImage2D(m_RendererID, specification.MipmapLevel, 0, 0, width, height, m_DataFormat, GL_UNSIGNED_BYTE, data);

	m_IsLoaded = true;
	m_Width = width, m_Height = height;

	stbi_image_free(data);
}

Texture2D::Texture2D(const Texture2DSpecification& specification)
	: m_Width(specification.Width), m_Height(specification.Height), m_MipmapLevel(specification.MipmapLevel)
{
	m_InternalFormat = ImageFormatToGLInternalFormat(specification.Format);
	m_DataFormat = ImageFormatToGLDataFormat(specification.Format);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, TextureFilterFormatToGLFilterFormat(specification.MinFilter));
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, TextureFilterFormatToGLFilterFormat(specification.MagFilter));

	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, TextureWrapFormatToGLWrapFormat(specification.WrapFormat));
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, TextureWrapFormatToGLWrapFormat(specification.WrapFormat));

	m_IsLoaded = true;
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture2D::SetData(void* data)
{
	glTextureSubImage2D(m_RendererID, m_MipmapLevel, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
}

void Texture2D::Bind(uint32_t slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glBindTextureUnit(slot, m_RendererID);
}