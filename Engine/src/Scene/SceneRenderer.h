#pragma once

#include "glm/glm.hpp"
#include "Renderer/Mesh.h"
#include "Renderer/SkinnedMesh.h"
#include "Renderer/Camera.h"
#include "Renderer/Material.h"
#include "Renderer/UniformBuffer.h"
#include "Renderer/Shader.h"

struct DirectionalLight
{

};

struct PointLight
{
	glm::vec3 Position;
};

class SceneRenderer
{
public:
	SceneRenderer();
	~SceneRenderer();

	void BeginScene(const Camera& camera);
	void EndScene();

	void SubmitMesh(Ref<Mesh> mesh, const std::vector<Ref<Material>>& materials, const glm::mat4& transform);
	void SubmitSkinnedMesh(Ref<SkinnedMesh> mesh, const std::vector<Ref<Material>>& materials, const glm::mat4& transform, Ref<Animation> animation, float animationTime);

private:
	void Initialize();
	void ShutDown();

private:
	Ref<Shader> m_ModelShader;
	Ref<Shader> m_SkinnedMeshShader;
	Ref<UniformBuffer> m_CameraDataUniformBuffer;
	Ref<UniformBuffer> m_BoneTransformUniformBuffer;
	
	struct MeshRenderData
	{
		Ref<Mesh> Mesh;
		Ref<Material> Material;
		uint32_t SubmeshIndex;
		glm::mat4 Transform;
	};

	struct SkinnedMeshRenderData
	{
		Ref<SkinnedMesh> Mesh;
		Ref<Material> Material;
		uint32_t SubmeshIndex;
		Ref<Animation> Animation;
		float AnimationTime;
		glm::mat4 Transform;
	};

	std::vector<MeshRenderData> m_OpaqueMeshRenderData;
	std::vector<MeshRenderData> m_TransparentMeshRenderData;
	std::vector<SkinnedMeshRenderData> m_OpaqueSkinnedMeshRenderData;
	std::vector<SkinnedMeshRenderData> m_TransparentSkinnedMeshRenderData;

	std::vector<glm::mat4> m_BoneTransformBuffer;
};