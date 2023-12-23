#include "pch.h"
#include "VertexArray.h"

#include <GL/glew.h>


static GLenum CastToOpenGLBaseType(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4:
    case ShaderDataType::Mat3:
    case ShaderDataType::Mat4:
        return GL_FLOAT;
    case ShaderDataType::Int:
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
    case ShaderDataType::Int4:
        return GL_INT;
    case ShaderDataType::Uint:
    case ShaderDataType::Uint2:
    case ShaderDataType::Uint3:
    case ShaderDataType::Uint4:
        return GL_UNSIGNED_INT;
    case ShaderDataType::Bool:
        return GL_BOOL;
    }
    CORE_ERROR("Shader data type not supported");
    return 0;
}

VertexArray::VertexArray()
{
    glCreateVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(Ref<VertexBuffer> vertexBuffer)
{
    if (vertexBuffer->GetLayout().GetElements().empty()) 
        CORE_ERROR("VertexBuffer has no layout");

    glBindVertexArray(m_RendererID);
    vertexBuffer->Bind();

    const auto& layout = vertexBuffer->GetLayout();
    for (const auto& element : layout)
    {
        switch (element.DataType)
        {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            {
                glEnableVertexAttribArray(m_VertexBufferIndex);
                glVertexAttribPointer(m_VertexBufferIndex, element.GetComponentCount(), CastToOpenGLBaseType(element.DataType),
                                      element.Normalized, layout.GetStride(), (const void*)element.Offset);
                m_VertexBufferIndex++;
                break;
            }
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::Uint:
            case ShaderDataType::Uint2:
            case ShaderDataType::Uint3:
            case ShaderDataType::Uint4:
            case ShaderDataType::Bool:
            {
                glEnableVertexAttribArray(m_VertexBufferIndex);
                glVertexAttribIPointer(m_VertexBufferIndex, element.GetComponentCount(), CastToOpenGLBaseType(element.DataType),
                                       layout.GetStride(), (const void*)element.Offset);
                m_VertexBufferIndex++;
                break;
            }
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
            {
                uint8_t count = element.GetComponentCount();
                for (uint8_t i = 0; i < count; i++)
                {
                    glEnableVertexAttribArray(m_VertexBufferIndex);
                    glVertexAttribPointer(m_VertexBufferIndex, count, CastToOpenGLBaseType(element.DataType),
                        element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
                        (const void*)(element.Offset + sizeof(float) * count * i));
                    glVertexAttribDivisor(m_VertexBufferIndex, 1);
                    m_VertexBufferIndex++;
                }
                break;
            }
            default:
                CORE_ERROR("Unknown shader data type");
        }
    }

    m_VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(Ref<IndexBuffer> indexBuffer)
{
    glBindVertexArray(m_RendererID);
    indexBuffer->Bind();

    m_IndexBuffer = indexBuffer;
}

Ref<VertexArray> VertexArray::Create()
{
    return CreateRef<VertexArray>();
}

