#include "pch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "UniformBuffer.h"
#include "RendererAPI.h"
#include "Shader.h"
#include "Texture.h"
#include "Core/IO.h"
#include "Core/Timer.h"

////////////////////////////////////////////////////////////////
//  BATCHING USES MEMORY ADDRESS TO DIFFERETIATE TWO TEXTURES //
////////////////////////////////////////////////////////////////


#define ORTHO_BIT 0x80000000U
#define REMOVE_ORTHO_BIT(x) (x & 0x7fffffffU)


enum Bindings : uint32_t
{
    CameraData = 0
};


struct CameraUniformBufferData
{
	glm::mat4 ViewProjectionMatrix;
    glm::mat4 OrthographicProjectionMatrix;
};



struct Renderer2DData
{
    static const uint32_t MaxTextureSlots = 16;

    // Quads
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        uint32_t TexIndex; // First bit to identiy 2D or 3D
    };

    struct QuadData
    {
        QuadVertex Vertices[4];
    };

    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;
    uint32_t MaxQuadCount = 20000;

    QuadData* QuadDataBufferBase = nullptr;
    QuadData* QuadDataBufferPtr = nullptr;


    Ref<Shader> QuadShader;

    uint32_t QuadCount = 0;
    std::vector<Ref<Texture2D>> QuadTextures;
    std::unordered_map<Ref<Texture2D>, uint32_t> QuadTextureLookup;
    Ref<Texture2D> WhiteTexture;


    // Circles
    struct CircleVertex
    {
        glm::vec3 Position;
        glm::vec2 LocalQuadCoord;
        glm::vec4 Color;
        float Thickness;
    };

    struct CircleData
    {
        glm::vec2 Position;
        glm::vec4 Color;
        float Radius;
        float Thickness;
    };

    Ref<VertexArray> CircleVertexArray;
    Ref<VertexBuffer> CircleVertexBuffer;
    uint32_t MaxCircleCount = 1000;
    Ref<Shader> CircleShader;

    CircleVertex* CircleVertexBufferBase = nullptr;
    CircleVertex* CircleVertexBufferPtr = nullptr;

    CircleData* CircleDataArray = nullptr;
    uint32_t CircleCount = 0;


    // Characters
    struct CharacterVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec4 Outline; // 3 float for color, 1 float for thickness
        glm::vec2 TexCoord;
        uint32_t TexIndex;
    };

    struct CharacterData
    {
        CharacterVertex Vertices[4];
    };

    Ref<VertexArray> CharacterVertexArray;
    Ref<VertexBuffer> CharacterVertexBuffer;
    uint32_t MaxCharacterCount = 1000;

    CharacterData* CharacterDataBufferBase = nullptr;
    CharacterData* CharacterDataBufferPtr = nullptr;


    Ref<Shader> CharacterShader;

    uint32_t CharacterCount = 0;
    std::vector<Ref<Texture2D>> FontAtlasTextures;
    std::unordered_map<Ref<Texture2D>, uint32_t> FontAtlasTextureLookup;


    // Uniform Buffer
    Ref<UniformBuffer> CameraUniformBuffer;
};
static Renderer2DData s_Data;

void Renderer2D::Initialize()
{
    // Quads
    {
        s_Data.QuadVertexArray = VertexArray::Create();
        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxQuadCount * sizeof(Renderer2DData::QuadData));
        s_Data.QuadVertexBuffer->SetLayout({
            { "a_Position" , ShaderDataType::Float3 },
            { "a_Color"    , ShaderDataType::Float4 },
            { "a_TexCoord" , ShaderDataType::Float2 },
            { "a_TexIndex" , ShaderDataType::Uint   }
        });

        int indexCount = s_Data.MaxQuadCount * 6;
        uint32_t* indices = new uint32_t[indexCount];

        int cnt = 0;
        for (int i = 0; i < indexCount; i += 6)
        {
            indices[i + 0] = cnt + 0;
            indices[i + 1] = cnt + 1;
            indices[i + 2] = cnt + 2;

            indices[i + 3] = cnt + 2;
            indices[i + 4] = cnt + 3;
            indices[i + 5] = cnt + 0;

            cnt += 4;
        }

        s_Data.QuadVertexArray->Bind();
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
        s_Data.QuadVertexArray->SetIndexBuffer(IndexBuffer::Create(indices, indexCount));
        delete[] indices;

        s_Data.QuadDataBufferBase = new Renderer2DData::QuadData[s_Data.MaxQuadCount];

        s_Data.WhiteTexture = Texture2D::Create();
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&whiteTextureData);

        s_Data.QuadShader = Shader::Create("../Engine/assets/shaders/Texture.glsl");

        // Set the texture uniform index
        s_Data.QuadShader->Bind();
        int slotIndices[Renderer2DData::MaxTextureSlots];
        for (int j = 0; j < Renderer2DData::MaxTextureSlots; j++)
            slotIndices[j] = j;

        s_Data.QuadShader->SetUniformIntArray("u_Textures", slotIndices, Renderer2DData::MaxTextureSlots);
    }

    // Circles
    {
        s_Data.CircleVertexArray = VertexArray::Create();
        s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxCircleCount * 4 * sizeof(Renderer2DData::CircleVertex));
        s_Data.CircleVertexBuffer->SetLayout({
            { "a_Position"       , ShaderDataType::Float3 },
            { "a_LocalQuadCoord" , ShaderDataType::Float2 },
            { "a_Color"          , ShaderDataType::Float4 },
            { "a_Thickness"      , ShaderDataType::Float  }
        });

        int indexCount = s_Data.MaxCircleCount * 6;
        uint32_t* indices = new uint32_t[indexCount];

        int cnt = 0;
        for (int i = 0; i < indexCount; i += 6)
        {
            indices[i + 0] = cnt + 0;
            indices[i + 1] = cnt + 1;
            indices[i + 2] = cnt + 2;

            indices[i + 3] = cnt + 2;
            indices[i + 4] = cnt + 3;
            indices[i + 5] = cnt + 0;

            cnt += 4;
        }
        
        s_Data.CircleVertexArray->Bind();
        s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
        s_Data.CircleVertexArray->SetIndexBuffer(IndexBuffer::Create(indices, indexCount));
        delete[] indices;

        s_Data.CircleVertexBufferBase = new Renderer2DData::CircleVertex[s_Data.MaxCircleCount * 4];
        s_Data.CircleDataArray = new Renderer2DData::CircleData[s_Data.MaxCircleCount];

        s_Data.CircleShader = Shader::Create("../Engine/assets/shaders/Circle.glsl");
    }

    // Characters
    {
        s_Data.CharacterVertexArray = VertexArray::Create();
        s_Data.CharacterVertexBuffer = VertexBuffer::Create(s_Data.MaxCharacterCount * sizeof(Renderer2DData::CharacterData));
        s_Data.CharacterVertexBuffer->SetLayout({
            { "a_Position" , ShaderDataType::Float3 },
            { "a_Color"    , ShaderDataType::Float4 },
            { "a_Border"   , ShaderDataType::Float4 },
            { "a_TexCoord" , ShaderDataType::Float2 },
            { "a_TexIndex" , ShaderDataType::Int    }
        });

        int indexCount = s_Data.MaxCharacterCount * 6;
        uint32_t* indices = new uint32_t[indexCount];

        int cnt = 0;
        for (int i = 0; i < indexCount; i += 6)
        {
            indices[i + 0] = cnt + 0;
            indices[i + 1] = cnt + 1;
            indices[i + 2] = cnt + 2;

            indices[i + 3] = cnt + 2;
            indices[i + 4] = cnt + 3;
            indices[i + 5] = cnt + 0;

            cnt += 4;
        }

        s_Data.CharacterVertexArray->Bind();
        s_Data.CharacterVertexArray->AddVertexBuffer(s_Data.CharacterVertexBuffer);
        s_Data.CharacterVertexArray->SetIndexBuffer(IndexBuffer::Create(indices, indexCount));
        delete[] indices;


        s_Data.CharacterDataBufferBase = new Renderer2DData::CharacterData[s_Data.MaxCharacterCount];

        s_Data.CharacterShader = Shader::Create("../Engine/assets/shaders/Character.glsl");

        // Set the texture uniform index
        s_Data.CharacterShader->Bind();
        int slotIndices[Renderer2DData::MaxTextureSlots];
        for (int j = 0; j < Renderer2DData::MaxTextureSlots; j++)
            slotIndices[j] = j;

        s_Data.CharacterShader->SetUniformIntArray("u_Textures", slotIndices, Renderer2DData::MaxTextureSlots);
    }

    // Uniform buffer
    s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(CameraUniformBufferData), CameraData);
}

void Renderer2D::ShutDown()
{
    delete[] s_Data.QuadDataBufferBase;

    delete[] s_Data.CircleVertexBufferBase;
    delete[] s_Data.CircleDataArray;

    delete[] s_Data.CharacterDataBufferBase;

    s_Data.QuadVertexArray.reset();
    s_Data.QuadVertexBuffer.reset();
    s_Data.CircleVertexArray.reset();
    s_Data.CircleVertexBuffer.reset();
    s_Data.CharacterVertexArray.reset();
    s_Data.CharacterVertexBuffer.reset();
    s_Data.QuadShader.reset();
    s_Data.CircleShader.reset();
    s_Data.CharacterShader.reset();
    s_Data.CameraUniformBuffer.reset();
    s_Data.WhiteTexture.reset();
}

void Renderer2D::BeginScene(const Camera& camera, glm::vec2 viewportSize)
{
    Reset();

    CameraUniformBufferData cameraData;
    cameraData.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    cameraData.OrthographicProjectionMatrix = glm::ortho(viewportSize.x * -0.5f, viewportSize.x * 0.5f, viewportSize.y * -0.5f, viewportSize.y * 0.5f, -1.0f, 1.0f);
    s_Data.CameraUniformBuffer->SetData(&cameraData, sizeof(CameraUniformBufferData));
}

void Renderer2D::EndScene()
{
    // Temp
    //Timer timer;

    FlushQuads();

    //CORE_ERROR("Quad count:{0}, Time(ms):{1}", s_Data.QuadCount, timer.Tick() * 1000.0);
    
    //s_Data.CircleShader->Bind();
    //
    //if (uint32_t indexCount = BatchCircles())
    //    FlushCircles(indexCount);
    
   // CORE_ERROR("{0}", timer.Tick() * 1000.0);

    FlushCharacters();
    
   // CORE_ERROR("Character count:{0}, Time(ms):{1}", s_Data.CharacterCount, timer.Tick() * 1000.0);

    // TODO : remove
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        CORE_ERROR("OpenGL error");
    }
}

void Renderer2D::FlushQuads()
{
    if (s_Data.QuadCount == 0)
        return;

    std::sort(s_Data.QuadDataBufferBase, s_Data.QuadDataBufferPtr, [](const Renderer2DData::QuadData& a, const Renderer2DData::QuadData& b){
        return REMOVE_ORTHO_BIT(a.Vertices[0].TexIndex) < REMOVE_ORTHO_BIT(b.Vertices[0].TexIndex);
    });

    int batchIndex = -1;
    std::vector<uint32_t> batchEntries;
    for (int i = 0; i < s_Data.QuadCount; i++)
    {
        auto& data = s_Data.QuadDataBufferBase[i];

        if ((int)(REMOVE_ORTHO_BIT(data.Vertices[0].TexIndex) / Renderer2DData::MaxTextureSlots) > batchIndex)
        {
            batchEntries.emplace_back(i);
            batchIndex++;
        }

        uint32_t isOrtho = data.Vertices[0].TexIndex & ORTHO_BIT;
        uint32_t normalizedTexIndex = isOrtho + REMOVE_ORTHO_BIT(data.Vertices[0].TexIndex) % Renderer2DData::MaxTextureSlots;

        for (int j = 0; j < 4; j++)
        {
            data.Vertices[j].TexIndex = normalizedTexIndex;
        }
    }

    s_Data.QuadVertexBuffer->SetData(s_Data.QuadDataBufferBase, s_Data.QuadCount * sizeof(Renderer2DData::QuadData));


    s_Data.QuadShader->Bind();
    s_Data.QuadVertexArray->Bind();

    batchIndex = 0;

    for (;batchIndex < batchEntries.size() - 1; batchIndex++)
    {
        for (int j = 0; j < Renderer2DData::MaxTextureSlots; j++)
        {
            s_Data.QuadTextures[batchIndex * Renderer2DData::MaxTextureSlots + j]->Bind(j);
        }
        uint32_t quadCount = batchEntries[batchIndex + 1] - batchEntries[batchIndex];
        RendererAPI::DrawIndexedOffset(quadCount * 6, 0, batchEntries[batchIndex] * 4);
    }

    // Final batch
    for (int i = 0; i < s_Data.QuadTextures.size() - batchIndex * Renderer2DData::MaxTextureSlots; i++)
    {
        s_Data.QuadTextures[batchIndex * Renderer2DData::MaxTextureSlots + i]->Bind(i);
    }
    uint32_t quadCount = s_Data.QuadCount - batchEntries.back();
    RendererAPI::DrawIndexedOffset(quadCount * 6, 0, batchEntries.back() * 4);
}


uint32_t Renderer2D::BatchCircles()
{
    if (s_Data.CircleCount == 0)
        return 0;

    s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;
    uint32_t indexCount = 0;

    // NOTE : Using 1.2f for extra fading spece
    constexpr glm::vec2 vertexPositions[] = { {-1.2f, 1.2f}, {1.2f, 1.2f}, {1.2f, -1.2f}, {-1.f, -1.2f} };

    for (uint32_t i = 0; i < s_Data.CircleCount; i++)
    {
        auto& circleData = s_Data.CircleDataArray[i];
        for (int j = 0; j < 4; j++)
        {
            s_Data.CircleVertexBufferPtr->Position = glm::vec3(circleData.Position + vertexPositions[j] * circleData.Radius, 0.f);
            s_Data.CircleVertexBufferPtr->LocalQuadCoord = vertexPositions[j];
            s_Data.CircleVertexBufferPtr->Color = circleData.Color;
            s_Data.CircleVertexBufferPtr->Thickness = circleData.Thickness;
            s_Data.CircleVertexBufferPtr++;
        }

        indexCount += 6;
    }

    return indexCount;
}

void Renderer2D::FlushCircles(uint32_t indexCount)
{
    uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
    s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);

    s_Data.CircleVertexArray->Bind();
    s_Data.CircleVertexBuffer->Bind();
    s_Data.CircleVertexArray->GetIndexBuffer()->Bind();
    RendererAPI::DrawIndexed(s_Data.CircleVertexArray, indexCount);
}

void Renderer2D::FlushCharacters()
{
    if (s_Data.CharacterCount == 0)
        return;


    std::sort(s_Data.CharacterDataBufferBase, s_Data.CharacterDataBufferPtr, [](const Renderer2DData::CharacterData& a, const Renderer2DData::CharacterData& b){
        return REMOVE_ORTHO_BIT(a.Vertices[0].TexIndex) < REMOVE_ORTHO_BIT(b.Vertices[0].TexIndex);        
    });


    int batchIndex = -1;
    std::vector<uint32_t> batchEntries;
    for (int i = 0; i < s_Data.CharacterCount; i++)
    {
        auto& data = s_Data.CharacterDataBufferBase[i];

        if ((int)(REMOVE_ORTHO_BIT(data.Vertices[0].TexIndex) / Renderer2DData::MaxTextureSlots) > batchIndex)
        {
            batchEntries.emplace_back(i);
            batchIndex++;
        }

        uint32_t isOrtho = data.Vertices[0].TexIndex & ORTHO_BIT;
        uint32_t normalizedTexIndex = isOrtho + REMOVE_ORTHO_BIT(data.Vertices[0].TexIndex) % Renderer2DData::MaxTextureSlots;

        for (int j = 0; j < 4; j++)
        {
            data.Vertices[j].TexIndex = normalizedTexIndex;
        }
    }

    s_Data.CharacterVertexBuffer->SetData(s_Data.CharacterDataBufferBase, s_Data.CharacterCount * sizeof(Renderer2DData::CharacterData));


    s_Data.CharacterShader->Bind();
    s_Data.CharacterVertexArray->Bind();
    RendererAPI::SetDepthTest(false);

    batchIndex = 0;

    for (; batchIndex < batchEntries.size() - 1; batchIndex++)
    {
        for (int j = 0; j < Renderer2DData::MaxTextureSlots; j++)
        {
            s_Data.FontAtlasTextures[batchIndex * Renderer2DData::MaxTextureSlots + j]->Bind(j);
        }
        uint32_t charCount = batchEntries[batchIndex + 1] - batchEntries[batchIndex];
        RendererAPI::DrawIndexedOffset(charCount * 6, 0, batchEntries[batchIndex] * 4);
    }

    // Final batch
    for (int i = 0; i < s_Data.FontAtlasTextures.size() - batchIndex * Renderer2DData::MaxTextureSlots; i++)
    {
        s_Data.FontAtlasTextures[batchIndex * Renderer2DData::MaxTextureSlots + i]->Bind(i);
    }
    uint32_t charCount = s_Data.CharacterCount - batchEntries.back();
    RendererAPI::DrawIndexedOffset(charCount * 6, 0, batchEntries.back() * 4);

    RendererAPI::SetDepthTest(true);
}

void Renderer2D::Reset()
{
    s_Data.QuadCount = 0;
    s_Data.CircleCount = 0;
    s_Data.CharacterCount = 0;

    s_Data.QuadTextures.clear();
    s_Data.QuadTextureLookup.clear();
    s_Data.FontAtlasTextures.clear();
    s_Data.FontAtlasTextureLookup.clear();

    s_Data.QuadDataBufferPtr = s_Data.QuadDataBufferBase;
    s_Data.CharacterDataBufferPtr = s_Data.CharacterDataBufferBase;

    s_Data.QuadTextures.emplace_back(s_Data.WhiteTexture);
    s_Data.QuadTextureLookup[s_Data.WhiteTexture] = 0;
}

void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color)
{
    DrawQuad(position, size, 0.0f, color);
}

void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, Ref<Texture2D> texture, glm::vec4 color)
{
    DrawQuad(position, size, 0.0f, texture, color);
}

void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color)
{
    constexpr glm::vec4 vertexPositions[4] = { { -0.5f,  0.5f, 0.0f, 1.0f  },
                                               {  0.5f,  0.5f, 0.0f, 1.0f  },
                                               {  0.5f, -0.5f, 0.0f, 1.0f  },
                                               { -0.5f, -0.5f, 0.0f, 1.0f  } };

    constexpr glm::vec2 texCoords[4] = { { 0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f} };
    
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) *
                          glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
                          glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

    for (int i = 0; i < 4; i++)
    {
        s_Data.QuadDataBufferPtr->Vertices[i].Position = transform * vertexPositions[i];
        s_Data.QuadDataBufferPtr->Vertices[i].Color = color;
        s_Data.QuadDataBufferPtr->Vertices[i].TexCoord = texCoords[i];
        s_Data.QuadDataBufferPtr->Vertices[i].TexIndex = ORTHO_BIT;
    }

    s_Data.QuadDataBufferPtr++;
    s_Data.QuadCount++;
}

void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, float rotation, Ref<Texture2D> texture, glm::vec4 color)
{
    uint32_t texIndex;

    auto it = s_Data.QuadTextureLookup.find(texture);
    if (it == s_Data.QuadTextureLookup.end())
    {
        texIndex = s_Data.QuadTextures.size();
        s_Data.QuadTextures.emplace_back(texture);
        s_Data.QuadTextureLookup[texture] = texIndex;
    }
    else
        texIndex = it->second;


    constexpr glm::vec4 vertexPositions[4] = { { -0.5f,  0.5f, 0.0f, 1.0f  },
                                               {  0.5f,  0.5f, 0.0f, 1.0f  },
                                               {  0.5f, -0.5f, 0.0f, 1.0f  },
                                               { -0.5f, -0.5f, 0.0f, 1.0f  } };

    constexpr glm::vec2 texCoords[4] = { { 0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f} };
    
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) *
                          glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
                          glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

    for (int i = 0; i < 4; i++)
    {
        s_Data.QuadDataBufferPtr->Vertices[i].Position = transform * vertexPositions[i];
        s_Data.QuadDataBufferPtr->Vertices[i].Color = color;
        s_Data.QuadDataBufferPtr->Vertices[i].TexCoord = texCoords[i];
        s_Data.QuadDataBufferPtr->Vertices[i].TexIndex = texIndex | ORTHO_BIT;
    }

    s_Data.QuadDataBufferPtr++;
    s_Data.QuadCount++;
}

void Renderer2D::DrawQuad(glm::vec2 size, const glm::mat4& transform, glm::vec4 color)
{
    constexpr glm::vec4 vertexPositions[4] = { { -0.5f,  0.5f, 0.0f, 1.0f  },
                                               {  0.5f,  0.5f, 0.0f, 1.0f  },
                                               {  0.5f, -0.5f, 0.0f, 1.0f  },
                                               { -0.5f, -0.5f, 0.0f, 1.0f  } };

    constexpr glm::vec2 texCoords[4] = { { 0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f} };

    
    glm::mat4 finalTransform = transform * glm::scale(glm::mat4(1.0f), glm::vec3(size, 0.0f));

    for (int i = 0; i < 4; i++)
    {
        s_Data.QuadDataBufferPtr->Vertices[i].Position = finalTransform * vertexPositions[i];
        s_Data.QuadDataBufferPtr->Vertices[i].Color = color;
        s_Data.QuadDataBufferPtr->Vertices[i].TexCoord = texCoords[i];
        s_Data.QuadDataBufferPtr->Vertices[i].TexIndex = 0;
    }

    s_Data.QuadDataBufferPtr++;
    s_Data.QuadCount++;
}

void Renderer2D::DrawQuad(glm::vec2 size, const glm::mat4& transform, Ref<Texture2D> texture, glm::vec4 color)
{
    uint32_t texIndex;

    auto it = s_Data.QuadTextureLookup.find(texture);
    if (it == s_Data.QuadTextureLookup.end())
    {
        texIndex = s_Data.QuadTextures.size();
        s_Data.QuadTextures.emplace_back(texture);
        s_Data.QuadTextureLookup[texture] = texIndex;
    }
    else
        texIndex = it->second;


    constexpr glm::vec4 vertexPositions[4] = { { -0.5f,  0.5f, 0.0f, 1.0f  },
                                               {  0.5f,  0.5f, 0.0f, 1.0f  },
                                               {  0.5f, -0.5f, 0.0f, 1.0f  },
                                               { -0.5f, -0.5f, 0.0f, 1.0f  } };

    constexpr glm::vec2 texCoords[4] = { { 0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f} };


    glm::mat4 finalTransform = transform * glm::scale(glm::mat4(1.0f), glm::vec3(size, 0.0f));

    for (int i = 0; i < 4; i++)
    {
        s_Data.QuadDataBufferPtr->Vertices[i].Position = finalTransform * vertexPositions[i];
        s_Data.QuadDataBufferPtr->Vertices[i].Color = color;
        s_Data.QuadDataBufferPtr->Vertices[i].TexCoord = texCoords[i];
        s_Data.QuadDataBufferPtr->Vertices[i].TexIndex = texIndex;
    }

    s_Data.QuadDataBufferPtr++;
    s_Data.QuadCount++;
}

void Renderer2D::DrawString(const std::string& text, Ref<Font> font, float fontSize, glm::vec2 position, glm::vec4 color)
{
    DrawString(text, font, fontSize, 0.0f, position, color, color);
}

void Renderer2D::DrawString(const std::string& text, Ref<Font> font, float fontSize, const glm::mat4& transform, glm::vec4 color)
{
    DrawString(text, font, fontSize, 0.0f, transform, color, color);
}

void Renderer2D::DrawString(const std::string& text, Ref<Font> font, float fontSize, float outlineThickness, glm::vec2 position, glm::vec4 color, glm::vec3 outlineColor)
{
    auto atlasTexture = font->GetAtlasTexture();
    float scale = fontSize / font->GetSize();

    uint32_t texIndex;

    auto it = s_Data.FontAtlasTextureLookup.find(atlasTexture);
    if (it == s_Data.FontAtlasTextureLookup.end())
    {
        texIndex = s_Data.FontAtlasTextures.size();
        s_Data.FontAtlasTextures.emplace_back(atlasTexture);
        s_Data.FontAtlasTextureLookup[atlasTexture] = texIndex;
    }
    else
        texIndex = it->second;


    glm::vec3 cursor = glm::vec3(position, 0.0f);

    for (char ch : text)
    {
        const GlyphMetric& metric = font->GetGlyphMetric(ch);

        s_Data.CharacterDataBufferPtr->Vertices[0].Position = cursor + glm::vec3(metric.Offset.x                , metric.Offset.y - metric.Size.y, 0.0f) * scale;
        s_Data.CharacterDataBufferPtr->Vertices[1].Position = cursor + glm::vec3(metric.Offset.x                , metric.Offset.y                , 0.0f) * scale;
        s_Data.CharacterDataBufferPtr->Vertices[2].Position = cursor + glm::vec3(metric.Offset.x + metric.Size.x, metric.Offset.y                , 0.0f) * scale;
        s_Data.CharacterDataBufferPtr->Vertices[3].Position = cursor + glm::vec3(metric.Offset.x + metric.Size.x, metric.Offset.y - metric.Size.y, 0.0f) * scale;

        s_Data.CharacterDataBufferPtr->Vertices[0].TexCoord = metric.TexCoords[0];
        s_Data.CharacterDataBufferPtr->Vertices[1].TexCoord = glm::vec2(metric.TexCoords[0].x, metric.TexCoords[1].y);
        s_Data.CharacterDataBufferPtr->Vertices[2].TexCoord = metric.TexCoords[1];
        s_Data.CharacterDataBufferPtr->Vertices[3].TexCoord = glm::vec2(metric.TexCoords[1].x, metric.TexCoords[0].y);

        s_Data.CharacterDataBufferPtr->Vertices[0].Color = color;
        s_Data.CharacterDataBufferPtr->Vertices[1].Color = color;
        s_Data.CharacterDataBufferPtr->Vertices[2].Color = color;
        s_Data.CharacterDataBufferPtr->Vertices[3].Color = color;

        s_Data.CharacterDataBufferPtr->Vertices[0].Outline = glm::vec4(outlineColor, outlineThickness);
        s_Data.CharacterDataBufferPtr->Vertices[1].Outline = glm::vec4(outlineColor, outlineThickness);
        s_Data.CharacterDataBufferPtr->Vertices[2].Outline = glm::vec4(outlineColor, outlineThickness);
        s_Data.CharacterDataBufferPtr->Vertices[3].Outline = glm::vec4(outlineColor, outlineThickness);

        s_Data.CharacterDataBufferPtr->Vertices[0].TexIndex = texIndex | ORTHO_BIT;
        s_Data.CharacterDataBufferPtr->Vertices[1].TexIndex = texIndex | ORTHO_BIT;
        s_Data.CharacterDataBufferPtr->Vertices[2].TexIndex = texIndex | ORTHO_BIT;
        s_Data.CharacterDataBufferPtr->Vertices[3].TexIndex = texIndex | ORTHO_BIT;


        s_Data.CharacterDataBufferPtr++;
        s_Data.CharacterCount++;

        cursor.x += metric.Advance * scale;
    }
}

void Renderer2D::DrawString(const std::string& text, Ref<Font> font, float fontSize, float outlineThickness, const glm::mat4& transform, glm::vec4 color, glm::vec3 outlineColor)
{
    auto atlasTexture = font->GetAtlasTexture();
    float scale = fontSize / font->GetSize();

    uint32_t texIndex;

    auto it = s_Data.FontAtlasTextureLookup.find(atlasTexture);
    if (it == s_Data.FontAtlasTextureLookup.end())
    {
        texIndex = s_Data.FontAtlasTextures.size();
        s_Data.FontAtlasTextures.emplace_back(atlasTexture);
        s_Data.FontAtlasTextureLookup[atlasTexture] = texIndex;
    }
    else
        texIndex = it->second;


    glm::vec4 cursor(0.0f, 0.0f, 0.0f, 1.0f);

    for (char ch : text)
    {
        const GlyphMetric& metric = font->GetGlyphMetric(ch);

        s_Data.CharacterDataBufferPtr->Vertices[0].Position = transform * (cursor + glm::vec4(metric.Offset.x                , metric.Offset.y - metric.Size.y, 0.0f, 0.0f) * scale);
        s_Data.CharacterDataBufferPtr->Vertices[1].Position = transform * (cursor + glm::vec4(metric.Offset.x                , metric.Offset.y                , 0.0f, 0.0f) * scale);
        s_Data.CharacterDataBufferPtr->Vertices[2].Position = transform * (cursor + glm::vec4(metric.Offset.x + metric.Size.x, metric.Offset.y                , 0.0f, 0.0f) * scale);
        s_Data.CharacterDataBufferPtr->Vertices[3].Position = transform * (cursor + glm::vec4(metric.Offset.x + metric.Size.x, metric.Offset.y - metric.Size.y, 0.0f, 0.0f) * scale);

        s_Data.CharacterDataBufferPtr->Vertices[0].TexCoord = metric.TexCoords[0];
        s_Data.CharacterDataBufferPtr->Vertices[1].TexCoord = glm::vec2(metric.TexCoords[0].x, metric.TexCoords[1].y);
        s_Data.CharacterDataBufferPtr->Vertices[2].TexCoord = metric.TexCoords[1];
        s_Data.CharacterDataBufferPtr->Vertices[3].TexCoord = glm::vec2(metric.TexCoords[1].x, metric.TexCoords[0].y);

        s_Data.CharacterDataBufferPtr->Vertices[0].Color = color;
        s_Data.CharacterDataBufferPtr->Vertices[1].Color = color;
        s_Data.CharacterDataBufferPtr->Vertices[2].Color = color;
        s_Data.CharacterDataBufferPtr->Vertices[3].Color = color;

        s_Data.CharacterDataBufferPtr->Vertices[0].Outline = glm::vec4(outlineColor, outlineThickness);
        s_Data.CharacterDataBufferPtr->Vertices[1].Outline = glm::vec4(outlineColor, outlineThickness);
        s_Data.CharacterDataBufferPtr->Vertices[2].Outline = glm::vec4(outlineColor, outlineThickness);
        s_Data.CharacterDataBufferPtr->Vertices[3].Outline = glm::vec4(outlineColor, outlineThickness);

        s_Data.CharacterDataBufferPtr->Vertices[0].TexIndex = texIndex | ORTHO_BIT;
        s_Data.CharacterDataBufferPtr->Vertices[1].TexIndex = texIndex | ORTHO_BIT;
        s_Data.CharacterDataBufferPtr->Vertices[2].TexIndex = texIndex | ORTHO_BIT;
        s_Data.CharacterDataBufferPtr->Vertices[3].TexIndex = texIndex | ORTHO_BIT;


        s_Data.CharacterDataBufferPtr++;
        s_Data.CharacterCount++;

        cursor.x += metric.Advance * scale;
    }
}

void Renderer2D::DrawString(const std::string& text, Ref<Font> font, float fontSize, glm::vec2 size, glm::vec2 alignment, glm::vec2 position, float rotation, glm::vec2 scale, glm::vec4 color, float outlineThickness, glm::vec3 outlineColor)
{
    auto atlasTexture = font->GetAtlasTexture();
    float fontScale = fontSize / font->GetSize();

    uint32_t texIndex;

    auto it = s_Data.FontAtlasTextureLookup.find(atlasTexture);
    if (it == s_Data.FontAtlasTextureLookup.end())
    {
        texIndex = s_Data.FontAtlasTextures.size();
        s_Data.FontAtlasTextures.emplace_back(atlasTexture);
        s_Data.FontAtlasTextureLookup[atlasTexture] = texIndex;
    }
    else
        texIndex = it->second;

    struct TextLine
    {
        size_t Begin = 0, End = 0;
        uint32_t Length = 0;
    };

    // Split to lines
    static std::vector<TextLine> textLines;
    textLines.clear();

    size_t tokenBegin = 0, tokenEnd = text.find_first_of(' '), lineBegin = 0;
    std::string_view textView(text);
    uint32_t totalLength = 0, lineCount = 0;
    uint32_t spaceAdvance = font->GetGlyphMetric(' ').Advance;

    while (tokenEnd != std::string::npos)
    {
        std::string_view token = textView.substr(tokenBegin, tokenEnd - tokenBegin);
        uint32_t length = 0;

        for (int i = 0; i < token.size(); i++)
        {
            const GlyphMetric& metric = font->GetGlyphMetric(token[i]);
            length += metric.Advance;
        }

        //length += font->GetGlyphMetric(token.back()).Size.x;


        if (totalLength + spaceAdvance + length > size.x) // Next line
        {
            TextLine& line = textLines.emplace_back();
            line.Begin = lineBegin;
            line.End = tokenBegin - 1;
            line.Length = totalLength;

            totalLength = 0;
            lineBegin = tokenBegin;
            lineCount++;
        }

        totalLength += length + spaceAdvance;
        tokenBegin = tokenEnd + 1;
        tokenEnd = text.find_first_of(' ', tokenBegin);
    }

    
    uint32_t totalHeight = fontSize * lineCount;


    // Turn the text into quad
    for (int i = 0; i < textLines.size(); i++) 
    {
        TextLine& line = textLines[i];

        glm::vec3 cursor(0.0f);
        cursor.x = position.x + glm::mix(-size.x / 2, size.x / 2 - line.Length, (alignment.x + 1) * 0.5f);
        cursor.y = position.y + size.y * alignment.y * 0.5f + totalHeight * (1.0f - alignment.y) * 0.5f - (i + 1) * fontSize;

        for (char ch : textView.substr(line.Begin, line.End - line.Begin))
        {
            const GlyphMetric& metric = font->GetGlyphMetric(ch);

            s_Data.CharacterDataBufferPtr->Vertices[0].Position = cursor + glm::vec3(metric.Offset.x                , metric.Offset.y - metric.Size.y, 0.0f) * glm::vec3(scale, 0.0f) * fontScale;
            s_Data.CharacterDataBufferPtr->Vertices[1].Position = cursor + glm::vec3(metric.Offset.x                , metric.Offset.y                , 0.0f) * glm::vec3(scale, 0.0f) * fontScale;
            s_Data.CharacterDataBufferPtr->Vertices[2].Position = cursor + glm::vec3(metric.Offset.x + metric.Size.x, metric.Offset.y                , 0.0f) * glm::vec3(scale, 0.0f) * fontScale;
            s_Data.CharacterDataBufferPtr->Vertices[3].Position = cursor + glm::vec3(metric.Offset.x + metric.Size.x, metric.Offset.y - metric.Size.y, 0.0f) * glm::vec3(scale, 0.0f) * fontScale;

            s_Data.CharacterDataBufferPtr->Vertices[0].TexCoord = metric.TexCoords[0];
            s_Data.CharacterDataBufferPtr->Vertices[1].TexCoord = glm::vec2(metric.TexCoords[0].x, metric.TexCoords[1].y);
            s_Data.CharacterDataBufferPtr->Vertices[2].TexCoord = metric.TexCoords[1];
            s_Data.CharacterDataBufferPtr->Vertices[3].TexCoord = glm::vec2(metric.TexCoords[1].x, metric.TexCoords[0].y);

            s_Data.CharacterDataBufferPtr->Vertices[0].Color = color;
            s_Data.CharacterDataBufferPtr->Vertices[1].Color = color;
            s_Data.CharacterDataBufferPtr->Vertices[2].Color = color;
            s_Data.CharacterDataBufferPtr->Vertices[3].Color = color;

            s_Data.CharacterDataBufferPtr->Vertices[0].Outline = glm::vec4(outlineColor, outlineThickness);
            s_Data.CharacterDataBufferPtr->Vertices[1].Outline = glm::vec4(outlineColor, outlineThickness);
            s_Data.CharacterDataBufferPtr->Vertices[2].Outline = glm::vec4(outlineColor, outlineThickness);
            s_Data.CharacterDataBufferPtr->Vertices[3].Outline = glm::vec4(outlineColor, outlineThickness);

            s_Data.CharacterDataBufferPtr->Vertices[0].TexIndex = texIndex | ORTHO_BIT;
            s_Data.CharacterDataBufferPtr->Vertices[1].TexIndex = texIndex | ORTHO_BIT;
            s_Data.CharacterDataBufferPtr->Vertices[2].TexIndex = texIndex | ORTHO_BIT;
            s_Data.CharacterDataBufferPtr->Vertices[3].TexIndex = texIndex | ORTHO_BIT;


            s_Data.CharacterDataBufferPtr++;
            s_Data.CharacterCount++;

            cursor.x += metric.Advance * fontScale;
        }
    }
}

void Renderer2D::DrawCircle(glm::vec2 position, float radius, float thickness, glm::vec4 color)
{
    s_Data.CircleDataArray[s_Data.CircleCount].Position = position;
    s_Data.CircleDataArray[s_Data.CircleCount].Radius = radius;
    s_Data.CircleDataArray[s_Data.CircleCount].Thickness = thickness;
    s_Data.CircleDataArray[s_Data.CircleCount].Color = color;

    s_Data.CircleCount++;
}
