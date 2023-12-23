#pragma once

#include <GL/glew.h>
#include <string>
#include "Core/Base.h"


enum class ImageFormat
{
	None = 0, R8, RGB8,	RGBA8, RGBA32F
};

enum class TextureWrapFormat
{
	None = 0, Repeat, ClampToEdge, ClampToBorder
};

enum class TextureFilterFormat
{
	None = 0, Linear, Nearest
};

struct Texture2DSpecification
{
	uint32_t Width = 1, Height = 1, MipmapLevel = 0;
	bool SRGB = true;
	ImageFormat Format = ImageFormat::RGBA8;
	TextureWrapFormat WrapFormat = TextureWrapFormat::Repeat;
	TextureFilterFormat MinFilter = TextureFilterFormat::Linear, MagFilter = TextureFilterFormat::Linear;
};

class Texture2D
{
public:
	Texture2D(const std::string& path, const Texture2DSpecification& specification = Texture2DSpecification());
	Texture2D(const Texture2DSpecification& specification = Texture2DSpecification());
	~Texture2D();

	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }


	void SetData(void* data);
	void Bind(uint32_t slot = 0) const;

	bool IsLoaded() const { return m_IsLoaded; }
	uint32_t GetRendererID() const { return m_RendererID; }


	static Ref<Texture2D> Create(const std::string& filepath, const Texture2DSpecification& specification = Texture2DSpecification());
	static Ref<Texture2D> Create(const Texture2DSpecification& specification = Texture2DSpecification());

private:
	bool m_IsLoaded = false;
	void* m_Data = nullptr;
	uint32_t m_Width = 0, m_Height = 0, m_MipmapLevel = 0;
	GLenum m_InternalFormat, m_DataFormat;
	uint32_t m_RendererID = 0;
};
