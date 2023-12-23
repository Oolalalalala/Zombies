#include "pch.h"
#include "Font.h"

#include "freetype/freetype.h"
#include "Renderer/RendererAPI.h"


Font::Font(const std::string& filepath, uint32_t fontSize)
    : m_Size(fontSize)
{
    FT_Library library;

	if (FT_Init_FreeType(&library))
	{
		CORE_ERROR("Failed to initialize freetype library");
	}

	FT_Face face;

	if (FT_New_Face(library, filepath.c_str(), 0, &face))
	{
		CORE_ERROR("Failed to load font, filepath = \"{0}\"", filepath);
		return;
	}

	if (FT_Set_Pixel_Sizes(face, 0, fontSize))
	{
		CORE_ERROR("Failed to set size of font, filepath = \"{0}\"", filepath);
	}



	int textureSize = 512;
    char* buffer = new char[textureSize * textureSize]();

	int padding = 0;
	int row = textureSize;
	int col = padding;
    uint32_t rowHeight = 0;

    
	for (FT_ULong glyphIdx = 32; glyphIdx < 127; glyphIdx++)
    {
        FT_UInt glyphIndex = FT_Get_Char_Index(face, glyphIdx);
        FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
        FT_Error error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_SDF);


        uint32_t width = face->glyph->bitmap.width;
        uint32_t height = face->glyph->bitmap.rows;
        rowHeight = std::max(height, rowHeight);
        
        if (col + face->glyph->bitmap.width + padding >= 512)
        {
            col = padding;
            row -= rowHeight + padding;
            rowHeight = 0;
        }

        for (unsigned int x = 0; x < width; x++)
        {
            for (unsigned int y = 0; y < height; y++)
            {
                if ((row - height + y) * textureSize + (col + x) >= 512 * 512) {
                    CORE_ERROR("{0}", (char)glyphIdx); continue;
                }
                //buffer[(row + y) * textureSize + col + x] = face->glyph->bitmap.buffer[(face->glyph->bitmap.rows - y - 1) * face->glyph->bitmap.width + x];
                //buffer[col + x][row - height + y] = face->glyph->bitmap.buffer[x][y];
                buffer[(row - height + y) * textureSize + (col + x)] = face->glyph->bitmap.buffer[(height - y - 1) * width + x];
            }
        }

        GlyphMetric& glyph = m_GlyphMetrices[glyphIdx];
        glyph.Advance = face->glyph->advance.x >> 6;
        glyph.Offset = { face->glyph->bitmap_left, face->glyph->bitmap_top };
        glyph.Size = { width, height };
        glyph.TexCoords[0] = {col / (float)textureSize, (row - height) / (float)textureSize};
        glyph.TexCoords[1] = {(col + width) / (float)textureSize, row / (float)textureSize};

        col += face->glyph->bitmap.width + padding;
    }

    // TODO : font height
    face->height;

    FT_Done_Face(face);
    FT_Done_FreeType(library);


	RendererAPI::SetPixelUnpackAlignment(1);

    Texture2DSpecification specs;
    specs.Width = textureSize;
    specs.Height = textureSize;
    specs.Format = ImageFormat::R8;

    m_AtlasTexture = Texture2D::Create(specs);
    m_AtlasTexture->SetData(buffer);

	RendererAPI::SetPixelUnpackAlignment(4);

    delete[] buffer;
}

Font::~Font()
{
}
