#include "pch.h"
#include "SceneRenderer.h"

#include "Renderer/RendererAPI.h"

enum Bindings : uint32_t
{
	// 0 is used by Renderer2D, reserved to prevent rebinding every frame
	CameraData = 1,
	BoneTransform = 2
};


struct CameraUniformBufferData
{
	glm::mat4 ViewProjectionMatrix = glm::mat4(0.0f);
	glm::vec3 CameraPosition = glm::vec3(0.0f);
};

SceneRenderer::SceneRenderer()
{
	Initialize();
}

SceneRenderer::~SceneRenderer()
{
	ShutDown();
}

void SceneRenderer::Initialize()
{
	m_ModelShader = Shader::Create("../Engine/assets/shaders/Model.glsl");
	m_ModelShader->SetUniformInt("u_DiffuseTexture", 0);

	m_SkinnedMeshShader = Shader::Create("../Engine/assets/shaders/SkinnedMesh.glsl");
	m_SkinnedMeshShader->SetUniformInt("u_DiffuseTexture", 0);
	

	//m_ModelShader = Shader::Create("../Engine/assets/shaders/Model-PBR.glsl");
	//m_ModelShader->SetUniformInt("u_AlbedoTexture", 0);
	//m_ModelShader->SetUniformInt("u_NormalTexture", 1);
	//m_ModelShader->SetUniformInt("u_MetallicTexture", 2);
	//m_ModelShader->SetUniformInt("u_RoughnessTexture", 3);
	//m_ModelShader->SetUniformInt("u_AOTexture", 4);
	//
	//m_ModelShader->SetUniformVec3("lightPositions[0]", glm::vec3(50, 0.0f, 0.0f));
	//m_ModelShader->SetUniformVec3("lightPositions[1]", glm::vec3(-40, 0.0f, 0.0f));
	//m_ModelShader->SetUniformVec3("lightPositions[2]", glm::vec3(-30, 0, 0.0f));
	//m_ModelShader->SetUniformVec3("lightPositions[3]", glm::vec3(-20, -0, 0.0f));
	//m_ModelShader->SetUniformVec3("lightColors[0]", glm::vec3(1.0f, 0.0f, 0.3f));
	//m_ModelShader->SetUniformVec3("lightColors[1]", glm::vec3(0.5f, 0.5f, 0.0f));
	//m_ModelShader->SetUniformVec3("lightColors[2]", glm::vec3(1.0f, 0.0f, 1.0f));
	//m_ModelShader->SetUniformVec3("lightColors[3]", glm::vec3(1.0f, 0.7f, 1.0f));

	m_CameraDataUniformBuffer = UniformBuffer::Create(sizeof(CameraUniformBufferData), CameraData);
	m_BoneTransformUniformBuffer = UniformBuffer::Create(256 * sizeof(glm::mat4), BoneTransform);
	m_BoneTransformBuffer.Allocate(256);
}

void SceneRenderer::ShutDown()
{
	m_BoneTransformBuffer.Release();
}

void SceneRenderer::BeginScene(const Camera& camera)
{
	m_MeshRenderData.clear();
	m_SkinnedMeshRenderData.clear();

	CameraUniformBufferData cameraData;
	cameraData.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	cameraData.CameraPosition = camera.GetPosition();
	m_CameraDataUniformBuffer->SetData(&cameraData, sizeof(CameraUniformBufferData));
}

void SceneRenderer::EndScene()
{
	m_ModelShader->Bind();

	for (auto& data : m_MeshRenderData)
	{
		m_ModelShader->SetUniformMat4("u_Model", data.Transform);

		auto mesh = data.Mesh;
		mesh->GetVertexArray()->Bind();

		for (auto& submesh : mesh->GetSubmeshes())
		{
			CORE_VERIFY(submesh.MaterialIndex < data.Materials.size(), "Material index greater than material table size");

			auto material = data.Materials[submesh.MaterialIndex];

			if (material->AlbedoTexture)             material->AlbedoTexture           ->Bind(0);
			if (material->NormalTexture)             material->NormalTexture           ->Bind(1);
			if (material->MetallicTexture)           material->MetallicTexture         ->Bind(2);
			if (material->RoughnessTexture)          material->RoughnessTexture        ->Bind(3);
			if (material->AmbientOcclusionTexture)   material->AmbientOcclusionTexture ->Bind(4);

			RendererAPI::DrawIndexedOffset(submesh.IndexCount, submesh.BaseIndex, submesh.BaseVertex);
		}
	}


	m_SkinnedMeshShader->Bind();

	for (auto& data : m_SkinnedMeshRenderData)
	{
		m_SkinnedMeshShader->SetUniformMat4("u_Model", data.Transform);

		auto skeleton = data.Mesh->GetSkeleton();
		skeleton->GetBoneTransforms(data.Animation, data.AnimationTime, m_BoneTransformBuffer);
		m_BoneTransformUniformBuffer->SetData(m_BoneTransformBuffer.Data, skeleton->GetBoneCount() * sizeof(glm::mat4));

		auto mesh = data.Mesh;
		mesh->GetVertexArray()->Bind();

		for (auto& submesh : mesh->GetSubmeshes())
		{
			CORE_VERIFY(submesh.MaterialIndex < data.Materials.size(), "Material index greater than material table size");

			auto material = data.Materials[submesh.MaterialIndex];

			if (material->AlbedoTexture)             material->AlbedoTexture           ->Bind(0);
			if (material->NormalTexture)             material->NormalTexture           ->Bind(1);
			if (material->MetallicTexture)           material->MetallicTexture         ->Bind(2);
			if (material->RoughnessTexture)          material->RoughnessTexture        ->Bind(3);
			if (material->AmbientOcclusionTexture)   material->AmbientOcclusionTexture ->Bind(4);

			RendererAPI::DrawIndexedOffset(submesh.IndexCount, submesh.BaseIndex, submesh.BaseVertex);
		}
	}
}

void SceneRenderer::SubmitMesh(Ref<Mesh> mesh, const std::vector<Ref<Material>>& materials, const glm::mat4& transform)
{
	auto& data = m_MeshRenderData.emplace_back();
	data.Mesh = mesh;
	data.Materials = materials;
	data.Transform = transform;
}

void SceneRenderer::SubmitSkinnedMesh(Ref<SkinnedMesh> mesh, const std::vector<Ref<Material>>& materials, const glm::mat4& transform, Ref<Animation> animation, float animationTime)
{
	auto& data = m_SkinnedMeshRenderData.emplace_back();
	data.Mesh = mesh;
	data.Materials = materials;
	data.Transform = transform;
	data.Animation = animation;
	data.AnimationTime = animationTime;
}

