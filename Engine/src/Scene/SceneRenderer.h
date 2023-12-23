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
		std::vector<Ref<Material>> Materials;
		glm::mat4 Transform;
	};

	struct SkinnedMeshRenderData
	{
		Ref<SkinnedMesh> Mesh;
		std::vector<Ref<Material>> Materials;
		glm::mat4 Transform;
		Ref<Animation> Animation;
		float AnimationTime;
	};

	std::vector<MeshRenderData> m_MeshRenderData;
	std::vector<SkinnedMeshRenderData> m_SkinnedMeshRenderData;

	Buffer<glm::mat4> m_BoneTransformBuffer;
};