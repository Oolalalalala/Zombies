#pragma once

#include "VertexArray.h"
#include "Skeleton.h"

struct SkinnedSubmesh
{
	uint32_t IndexCount = 0;
	uint32_t BaseVertex = 0;
	uint32_t BaseIndex = 0;
	uint32_t MaterialIndex = 0xFFFFFFFF;
};

struct SkinnedMeshData
{
	std::vector<glm::vec3> Positions;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec2> TexCoords;
	std::vector<glm::ivec4> BoneIndex;
	std::vector<glm::vec4> BoneWeight;
	std::vector<uint32_t> Indices;
	std::vector<SkinnedSubmesh> Submeshes;
	Ref<Skeleton> Skeleton;
	std::vector<Ref<Animation>> Animations;
};

class SkinnedMesh
{
public:
	SkinnedMesh(const SkinnedMeshData& meshData);
	~SkinnedMesh();

	Ref<VertexArray> GetVertexArray() { return m_VertexArray; }
	Ref<Skeleton> GetSkeleton() { return m_Skeleton; }
	Ref<Animation> GetAnimation(uint32_t index) { return m_Animations[index]; }
	std::vector<SkinnedSubmesh>& GetSubmeshes() { return m_Submeshes; }

private:
	std::vector<SkinnedSubmesh> m_Submeshes;
	std::vector<Ref<Animation>> m_Animations;

	Ref<VertexArray> m_VertexArray;
	Ref<Skeleton> m_Skeleton;
};