#include "pch.h"
#include "SkinnedMesh.h"

SkinnedMesh::SkinnedMesh(const SkinnedMeshData& meshData)
{
	m_VertexArray = VertexArray::Create();
	m_VertexArray->Bind();

	auto positionBuffer = VertexBuffer::Create((float*)meshData.Positions.data(), meshData.Positions.size() * sizeof(glm::vec3));
	auto normalBuffer = VertexBuffer::Create((float*)meshData.Normals.data(), meshData.Normals.size() * sizeof(glm::vec3));
	auto texCoordBuffer = VertexBuffer::Create((float*)meshData.TexCoords.data(), meshData.TexCoords.size() * sizeof(glm::vec2));
	auto boneIndexBuffer = VertexBuffer::Create((float*)meshData.BoneIndex.data(), meshData.BoneIndex.size() * sizeof(glm::ivec4));
	auto boneWeightBuffer = VertexBuffer::Create((float*)meshData.BoneWeight.data(), meshData.BoneWeight.size() * sizeof(glm::vec4));
	auto indexBuffer = IndexBuffer::Create(meshData.Indices.data(), meshData.Indices.size());

	positionBuffer->SetLayout({ {"a_Position", ShaderDataType::Float3} });
	normalBuffer->SetLayout({ {"a_Normal", ShaderDataType::Float3 } });
	texCoordBuffer->SetLayout({ {"a_TexCoord", ShaderDataType::Float2} });
	boneIndexBuffer->SetLayout({ {"a_BoneIndex", ShaderDataType::Int4} });
	boneWeightBuffer->SetLayout({ {"a_BoneWeight", ShaderDataType::Float4} });

	m_VertexArray->AddVertexBuffer(positionBuffer);
	m_VertexArray->AddVertexBuffer(normalBuffer);
	m_VertexArray->AddVertexBuffer(texCoordBuffer);
	m_VertexArray->AddVertexBuffer(boneIndexBuffer);
	m_VertexArray->AddVertexBuffer(boneWeightBuffer);
	m_VertexArray->SetIndexBuffer(indexBuffer);

	m_Submeshes = meshData.Submeshes;
	m_Animations = meshData.Animations;
	m_Skeleton = meshData.Skeleton;

}

SkinnedMesh::~SkinnedMesh()
{
}
