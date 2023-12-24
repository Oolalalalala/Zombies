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
	m_BoneTransformBuffer.resize(256); // The current max bone count is 256
}

void SceneRenderer::ShutDown()
{
}

void SceneRenderer::BeginScene(const Camera& camera)
{
	m_OpaqueMeshRenderData.clear();
	m_TransparentMeshRenderData.clear();
	m_OpaqueSkinnedMeshRenderData.clear();
	m_TransparentSkinnedMeshRenderData.clear();

	CameraUniformBufferData cameraData;
	cameraData.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	cameraData.CameraPosition = camera.GetPosition();
	m_CameraDataUniformBuffer->SetData(&cameraData, sizeof(CameraUniformBufferData));
}

void SceneRenderer::EndScene()
{
	// To minimize repetitive computations and changes
	Ref<Mesh> lastMesh = nullptr;
	Ref<SkinnedMesh> lastSkinnedMesh = nullptr; 

	m_ModelShader->Bind();

	for (auto& data : m_OpaqueMeshRenderData)
	{
		if (lastMesh != data.Mesh)
		{
			data.Mesh->GetVertexArray()->Bind();
			lastMesh = data.Mesh;
		}

		m_ModelShader->SetUniformMat4("u_Model", data.Transform);
		m_ModelShader->SetUniformFloat4("u_Color", data.Material->Color);

		if (data.Material->AlbedoTexture)             data.Material->AlbedoTexture           ->Bind(0);
		if (data.Material->NormalTexture)             data.Material->NormalTexture           ->Bind(1);
		if (data.Material->MetallicTexture)           data.Material->MetallicTexture         ->Bind(2);
		if (data.Material->RoughnessTexture)          data.Material->RoughnessTexture        ->Bind(3);
		if (data.Material->AmbientOcclusionTexture)   data.Material->AmbientOcclusionTexture ->Bind(4);

		auto& submesh = data.Mesh->GetSubmeshes()[data.SubmeshIndex];
		RendererAPI::DrawIndexedOffset(submesh.IndexCount, submesh.BaseIndex, submesh.BaseVertex);
	}


	m_SkinnedMeshShader->Bind();

	for (auto& data : m_OpaqueSkinnedMeshRenderData)
	{
		if (lastSkinnedMesh != data.Mesh)
		{
			data.Mesh->GetVertexArray()->Bind();

			auto skeleton = data.Mesh->GetSkeleton();
			skeleton->GetBoneTransforms(data.Animation, data.AnimationTime, m_BoneTransformBuffer);
			m_BoneTransformUniformBuffer->SetData(m_BoneTransformBuffer.data(), skeleton->GetBoneCount() * sizeof(glm::mat4));

			lastSkinnedMesh = data.Mesh;
		}

		m_SkinnedMeshShader->SetUniformMat4("u_Model", data.Transform);
		m_SkinnedMeshShader->SetUniformFloat4("u_Color", data.Material->Color);

		if (data.Material->AlbedoTexture)             data.Material->AlbedoTexture           ->Bind(0);
		if (data.Material->NormalTexture)             data.Material->NormalTexture           ->Bind(1);
		if (data.Material->MetallicTexture)           data.Material->MetallicTexture         ->Bind(2);
		if (data.Material->RoughnessTexture)          data.Material->RoughnessTexture        ->Bind(3);
		if (data.Material->AmbientOcclusionTexture)   data.Material->AmbientOcclusionTexture ->Bind(4);

		auto& submesh = data.Mesh->GetSubmeshes()[data.SubmeshIndex];
		RendererAPI::DrawIndexedOffset(submesh.IndexCount, submesh.BaseIndex, submesh.BaseVertex);
	}

	
	// To minimize repetitive computations and changes
	lastMesh = nullptr;
	lastSkinnedMesh = nullptr; 

	m_ModelShader->Bind();

	for (auto& data : m_TransparentMeshRenderData)
	{
		if (lastMesh != data.Mesh)
		{
			data.Mesh->GetVertexArray()->Bind();
			lastMesh = data.Mesh;
		}

		m_ModelShader->SetUniformMat4("u_Model", data.Transform);
		m_ModelShader->SetUniformFloat4("u_Color", data.Material->Color);

		if (data.Material->AlbedoTexture)             data.Material->AlbedoTexture           ->Bind(0);
		if (data.Material->NormalTexture)             data.Material->NormalTexture           ->Bind(1);
		if (data.Material->MetallicTexture)           data.Material->MetallicTexture         ->Bind(2);
		if (data.Material->RoughnessTexture)          data.Material->RoughnessTexture        ->Bind(3);
		if (data.Material->AmbientOcclusionTexture)   data.Material->AmbientOcclusionTexture ->Bind(4);

		auto& submesh = data.Mesh->GetSubmeshes()[data.SubmeshIndex];
		RendererAPI::DrawIndexedOffset(submesh.IndexCount, submesh.BaseIndex, submesh.BaseVertex);
	}


	m_SkinnedMeshShader->Bind();

	for (auto& data : m_TransparentSkinnedMeshRenderData)
	{
		if (lastSkinnedMesh != data.Mesh)
		{
			data.Mesh->GetVertexArray()->Bind();

			auto skeleton = data.Mesh->GetSkeleton();
			skeleton->GetBoneTransforms(data.Animation, data.AnimationTime, m_BoneTransformBuffer);
			m_BoneTransformUniformBuffer->SetData(m_BoneTransformBuffer.data(), skeleton->GetBoneCount() * sizeof(glm::mat4));

			lastSkinnedMesh = data.Mesh;
		}
		
		m_SkinnedMeshShader->SetUniformMat4("u_Model", data.Transform);
		m_SkinnedMeshShader->SetUniformFloat4("u_Color", data.Material->Color);

		if (data.Material->AlbedoTexture)             data.Material->AlbedoTexture           ->Bind(0);
		if (data.Material->NormalTexture)             data.Material->NormalTexture           ->Bind(1);
		if (data.Material->MetallicTexture)           data.Material->MetallicTexture         ->Bind(2);
		if (data.Material->RoughnessTexture)          data.Material->RoughnessTexture        ->Bind(3);
		if (data.Material->AmbientOcclusionTexture)   data.Material->AmbientOcclusionTexture ->Bind(4);

		auto& submesh = data.Mesh->GetSubmeshes()[data.SubmeshIndex];
		RendererAPI::DrawIndexedOffset(submesh.IndexCount, submesh.BaseIndex, submesh.BaseVertex);
	}
}

void SceneRenderer::SubmitMesh(Ref<Mesh> mesh, const std::vector<Ref<Material>>& materials, const glm::mat4& transform)
{
	auto& submeshes = mesh->GetSubmeshes();
	for (uint32_t i = 0; i < submeshes.size(); i++)
	{
		CORE_VERIFY(submeshes[i].MaterialIndex < materials.size(), "Material index greater than material table size");
		auto& material = materials[submeshes[i].MaterialIndex];

		auto& data = material->Color.a == 1.0f ? m_OpaqueMeshRenderData.emplace_back() : m_TransparentMeshRenderData.emplace_back();
		data.Mesh = mesh;
		data.Material = material;
		data.SubmeshIndex = i;
		data.Transform = transform;
	}
}

void SceneRenderer::SubmitSkinnedMesh(Ref<SkinnedMesh> mesh, const std::vector<Ref<Material>>& materials, const glm::mat4& transform, Ref<Animation> animation, float animationTime)
{
	auto& submeshes = mesh->GetSubmeshes();
	for (uint32_t i = 0; i < submeshes.size(); i++)
	{
		CORE_VERIFY(submeshes[i].MaterialIndex < materials.size(), "Material index greater than material table size");
		auto& material = materials[submeshes[i].MaterialIndex];

		auto& data = material->Color.a == 1.0f ? m_OpaqueSkinnedMeshRenderData.emplace_back() : m_TransparentSkinnedMeshRenderData.emplace_back();
		data.Mesh = mesh;
		data.Material = material;
		data.SubmeshIndex = i;
		data.Animation = animation;
		data.AnimationTime = animationTime;
		data.Transform = transform;
	}
}

