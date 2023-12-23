#include "pch.h"
#include "Mesh.h"

#include <gl/glew.h>
#include "RendererAPI.h"


Mesh::Mesh(const MeshData& meshData)
{
	m_VertexArray = VertexArray::Create();
	m_VertexArray->Bind();

	auto positionBuffer = VertexBuffer::Create((float*)meshData.Positions.data(), meshData.Positions.size() * sizeof(glm::vec3));
	auto normalBuffer = VertexBuffer::Create((float*)meshData.Normals.data(), meshData.Normals.size() * sizeof(glm::vec3));
	auto texCoordBuffer = VertexBuffer::Create((float*)meshData.TexCoords.data(), meshData.TexCoords.size() * sizeof(glm::vec2));
	auto indexBuffer = IndexBuffer::Create(meshData.Indices.data(), meshData.Indices.size());

	positionBuffer->SetLayout({ {"a_Position", ShaderDataType::Float3} });
	normalBuffer->SetLayout({ {"a_Normal", ShaderDataType::Float3 } });
	texCoordBuffer->SetLayout({ {"a_TexCoord", ShaderDataType::Float2} });

	m_VertexArray->AddVertexBuffer(positionBuffer);
	m_VertexArray->AddVertexBuffer(normalBuffer);
	m_VertexArray->AddVertexBuffer(texCoordBuffer);
	m_VertexArray->SetIndexBuffer(indexBuffer);

	m_Submeshes = meshData.Submeshes;
}

Mesh::~Mesh()
{
}