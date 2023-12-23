#pragma once

#include "Core/Base.h"
#include "Renderer/VertexArray.h"
#include "glm/glm.hpp"
#include <vector>


struct Submesh
{
	uint32_t IndexCount = 0;
	uint32_t BaseVertex = 0;
	uint32_t BaseIndex = 0;
	uint32_t MaterialIndex = 0xFFFFFFFF;
};

struct MeshData
{
	std::vector<glm::vec3> Positions;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec2> TexCoords;
	std::vector<uint32_t> Indices;
	std::vector<Submesh> Submeshes;
};

class Mesh
{
public:
	Mesh(const MeshData& meshData);
	~Mesh();

	Ref<VertexArray> GetVertexArray() { return m_VertexArray; }

	std::vector<Submesh>& GetSubmeshes() { return m_Submeshes; }

private:
	std::vector<Submesh> m_Submeshes;

	Ref<VertexArray> m_VertexArray;
};