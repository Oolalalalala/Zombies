#pragma once

#include "Texture.h"
#include <map>


struct GlyphMetric
{
	uint32_t Advance;
	glm::ivec2 Offset;
	glm::ivec2 Size;
	glm::vec2 TexCoords[2]; // 0: Bottom-Left, 1: Top-Right
};

class Font
{
public:
	Font(const std::string& filepath, uint32_t fontSize);
	~Font();

	Ref<Texture2D> GetAtlasTexture() { return m_AtlasTexture; }
	inline const GlyphMetric& GetGlyphMetric(char character) { return m_GlyphMetrices[character]; }
	uint32_t GetSize() { return m_Size; }

private:
	Ref<Texture2D> m_AtlasTexture;
	std::map<char, GlyphMetric> m_GlyphMetrices;

	uint32_t m_Size;
};