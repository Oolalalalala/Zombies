#include "pch.h"
#include "Model.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "Entity.h"
#include "Component.h"
#include "Renderer/Mesh.h"
#include "Asset/AssetManager.h"
#include "glm/gtc/type_ptr.hpp"




Entity Model::Load(Ref<Scene> scene, const std::string& filepath)
{
	CORE_TRACE("Loading model, filepath = \"{0}\"", filepath);

	Assimp::Importer importer;

	uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices;
	const aiScene* fileScene = importer.ReadFile(AssetManager::GetFilepath(filepath).string(), flags);

	if (!fileScene)
	{
		CORE_ERROR("Failed to load model, filepath = \"{0}\"\nError : {1}", filepath, importer.GetErrorString());
		return {};
	}

	Entity model = scene->CreateEntity(filepath);
	auto& meshRenderer = model.AddComponent<MeshRendererComponent>();
	auto& materialTable = model.AddComponent<MaterialTableComponent>();

	CORE_TRACE("Model has {0} submeshes, {1} materials", fileScene->mNumMeshes, fileScene->mNumMaterials);

	// Generate mesh
	MeshData meshData;
	meshData.Submeshes.resize(fileScene->mNumMeshes);

	uint32_t vertexCount = 0, indexCount = 0;
	for (uint32_t i = 0; i < fileScene->mNumMeshes; i++)
	{
		const aiMesh* submesh = fileScene->mMeshes[i];
		meshData.Submeshes[i].BaseVertex = vertexCount;
		meshData.Submeshes[i].BaseIndex = indexCount;
		meshData.Submeshes[i].IndexCount = submesh->mNumFaces * 3;
		meshData.Submeshes[i].MaterialIndex = submesh->mMaterialIndex;
		
		vertexCount += submesh->mNumVertices;
		indexCount += meshData.Submeshes[i].IndexCount;
	}

	meshData.Positions.reserve(vertexCount);
	meshData.Normals.reserve(vertexCount);
	meshData.TexCoords.reserve(vertexCount);
	meshData.Indices.reserve(indexCount);

	// Populate the buffers
	for (uint32_t i = 0; i < fileScene->mNumMeshes; i++)
	{
		const aiMesh* submesh = fileScene->mMeshes[i];

		// Vertex attributes
		for (uint32_t j = 0; j < submesh->mNumVertices; j++)
		{
			const aiVector3D& position = submesh->mVertices[j];
			const aiVector3D& normal = submesh->mNormals[j];
			const aiVector3D& texCoord = submesh->HasTextureCoords(0) ? submesh->mTextureCoords[0][j] : aiVector3D(0.f, 0.f, 0.f);

			meshData.Positions.emplace_back(position.x, position.y, position.z);
			meshData.Normals.emplace_back(normal.x, normal.y, normal.z);
			meshData.TexCoords.emplace_back(texCoord.x, texCoord.y);
		}

		// Index buffer
		for (uint32_t j = 0; j < submesh->mNumFaces; j++)
		{
			const aiFace& face = submesh->mFaces[j];

			meshData.Indices.emplace_back(face.mIndices[0]);
			meshData.Indices.emplace_back(face.mIndices[1]);
			meshData.Indices.emplace_back(face.mIndices[2]);
		}
	}

	meshRenderer.Mesh = AssetManager::CreateMesh(meshData);


	materialTable.Materials.resize(fileScene->mNumMaterials);

	for (int i = 0; i < fileScene->mNumMaterials; i++)
	{
		const aiMaterial* material = fileScene->mMaterials[i];

		AssetHandle albedoTexture = 0;
		AssetHandle normalTexture = 0;
		AssetHandle metallicTexture = 0;
		AssetHandle roughnessTexture = 0;
		AssetHandle ambientOcclusionTexture = 0;


		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{			
			aiString path;

			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				const aiTexture* embeddedTexture = fileScene->GetEmbeddedTexture(path.C_Str());

				if (embeddedTexture)
				{
					albedoTexture = AssetManager::CreateTexture((unsigned char*)embeddedTexture->pcData, embeddedTexture->mWidth, embeddedTexture->mHeight);
				}
				else
				{
					std::filesystem::path texturePath = std::filesystem::path(filepath).parent_path() / path.C_Str();
					albedoTexture = AssetManager::LoadTexture(texturePath.string());
				}
			}
			else
				CORE_ERROR("Failed to retrieve albedo texture path from ASSIMP");

		}
		
		if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
		{			
			aiString path;

			if (material->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS)
			{
				const aiTexture* embeddedTexture = fileScene->GetEmbeddedTexture(path.C_Str());

				if (embeddedTexture)
				{
					normalTexture = AssetManager::CreateTexture((unsigned char*)embeddedTexture->pcData, embeddedTexture->mWidth, embeddedTexture->mHeight);
				}
				else
				{
					std::filesystem::path texturePath = std::filesystem::path(filepath).parent_path() / path.C_Str();
					normalTexture = AssetManager::LoadTexture(texturePath.string());
				}
			}
			else
				CORE_ERROR("Failed to retrieve normal texture path from ASSIMP");

		}

		if (material->GetTextureCount(aiTextureType_METALNESS) > 0)
		{			
			aiString path;

			if (material->GetTexture(aiTextureType_METALNESS, 0, &path) == AI_SUCCESS)
			{
				const aiTexture* embeddedTexture = fileScene->GetEmbeddedTexture(path.C_Str());

				if (embeddedTexture)
				{
					metallicTexture = AssetManager::CreateTexture((unsigned char*)embeddedTexture->pcData, embeddedTexture->mWidth, embeddedTexture->mHeight);
				}
				else
				{
					std::filesystem::path texturePath = std::filesystem::path(filepath).parent_path() / path.C_Str();
					metallicTexture = AssetManager::LoadTexture(texturePath.string());
				}
			}
			else
				CORE_ERROR("Failed to retrieve metallic texture path from ASSIMP");

		}

		if (material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0)
		{			
			aiString path;

			if (material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &path) == AI_SUCCESS)
			{
				const aiTexture* embeddedTexture = fileScene->GetEmbeddedTexture(path.C_Str());

				if (embeddedTexture)
				{
					roughnessTexture = AssetManager::CreateTexture((unsigned char*)embeddedTexture->pcData, embeddedTexture->mWidth, embeddedTexture->mHeight);
				}
				else
				{
					std::filesystem::path texturePath = std::filesystem::path(filepath).parent_path() / path.C_Str();
					roughnessTexture = AssetManager::LoadTexture(texturePath.string());
				}
			}
			else
				CORE_ERROR("Failed to retrieve roughness texture path from ASSIMP");
		}
		
		if (material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) > 0)
		{			
			aiString path;

			if (material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &path) == AI_SUCCESS)
			{
				const aiTexture* embeddedTexture = fileScene->GetEmbeddedTexture(path.C_Str());

				if (embeddedTexture)
				{
					ambientOcclusionTexture = AssetManager::CreateTexture((unsigned char*)embeddedTexture->pcData, embeddedTexture->mWidth, embeddedTexture->mHeight);
				}
				else
				{
					std::filesystem::path texturePath = std::filesystem::path(filepath).parent_path() / path.C_Str();
					ambientOcclusionTexture = AssetManager::LoadTexture(texturePath.string());
				}
			}
			else
				CORE_ERROR("Failed to retrieve ambient occlusion texture path from ASSIMP");
		}

		CORE_TRACE("Material #{0} has {1} diffuse texture, {2} normal textures, {3} metallic texture, {4} roughness texture",
			i, material->GetTextureCount(aiTextureType_DIFFUSE), material->GetTextureCount(aiTextureType_NORMALS), material->GetTextureCount(aiTextureType_METALNESS), material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS));

		materialTable.Materials[i] = AssetManager::CreateMaterial(albedoTexture, normalTexture, metallicTexture, roughnessTexture, ambientOcclusionTexture);
	}

	importer.FreeScene();
	
	CORE_TRACE("Finished loading model");

	return model;
}

Entity Model::Load(Ref<Scene> scene, const std::string& filepath, const std::vector<AssetHandle>& materials)
{
	CORE_TRACE("Loading model, filepath = \"{0}\"", filepath);

	Assimp::Importer importer;

	uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices;
	const aiScene* fileScene = importer.ReadFile(AssetManager::GetFilepath(filepath).string(), flags);

	if (!fileScene)
	{
		CORE_ERROR("Failed to load model, filepath = \"{0}\"\nError : {1}", filepath, importer.GetErrorString());
		return {};
	}

	Entity model = scene->CreateEntity(filepath);
	auto& meshRenderer = model.AddComponent<MeshRendererComponent>();
	auto& materialTable = model.AddComponent<MaterialTableComponent>();

	CORE_TRACE("Model has {0} submeshes, {1} materials", fileScene->mNumMeshes, fileScene->mNumMaterials);

	// Generate mesh
	MeshData meshData;
	meshData.Submeshes.resize(fileScene->mNumMeshes);

	uint32_t vertexCount = 0, indexCount = 0;
	for (uint32_t i = 0; i < fileScene->mNumMeshes; i++)
	{
		const aiMesh* submesh = fileScene->mMeshes[i];
		meshData.Submeshes[i].BaseVertex = vertexCount;
		meshData.Submeshes[i].BaseIndex = indexCount;
		meshData.Submeshes[i].IndexCount = submesh->mNumFaces * 3;
		meshData.Submeshes[i].MaterialIndex = submesh->mMaterialIndex;
		
		vertexCount += submesh->mNumVertices;
		indexCount += meshData.Submeshes[i].IndexCount;
	}

	meshData.Positions.reserve(vertexCount);
	meshData.Normals.reserve(vertexCount);
	meshData.TexCoords.reserve(vertexCount);
	meshData.Indices.reserve(indexCount);

	// Populate the buffers
	for (uint32_t i = 0; i < fileScene->mNumMeshes; i++)
	{
		const aiMesh* submesh = fileScene->mMeshes[i];

		// Vertex attributes
		for (uint32_t j = 0; j < submesh->mNumVertices; j++)
		{
			const aiVector3D& position = submesh->mVertices[j];
			const aiVector3D& normal = submesh->mNormals[j];
			const aiVector3D& texCoord = submesh->HasTextureCoords(0) ? submesh->mTextureCoords[0][j] : aiVector3D(0.f, 0.f, 0.f);

			meshData.Positions.emplace_back(position.x, position.y, position.z);
			meshData.Normals.emplace_back(normal.x, normal.y, normal.z);
			meshData.TexCoords.emplace_back(texCoord.x, texCoord.y);
		}

		// Index buffer
		for (uint32_t j = 0; j < submesh->mNumFaces; j++)
		{
			const aiFace& face = submesh->mFaces[j];

			meshData.Indices.emplace_back(face.mIndices[0]);
			meshData.Indices.emplace_back(face.mIndices[1]);
			meshData.Indices.emplace_back(face.mIndices[2]);
		}
	}

	meshRenderer.Mesh = AssetManager::CreateMesh(meshData);


	CORE_VERIFY(materials.size(), "No material provided");


	materialTable.Materials.resize(fileScene->mNumMaterials);

	for (int i = 0; i < fileScene->mNumMaterials; i++)
	{
		if (i < materials.size())
			materialTable.Materials[i] = materials[i];
		else
		{
			materialTable.Materials[i] = materials.back();
			CORE_WARN("Model requires {0} materials, only {1} materials were provided", fileScene->mNumMaterials, materials.size());
		}
	}
	
	importer.FreeScene();
	
	CORE_TRACE("Finished loading model");

	return model;
}

static inline glm::mat4 mat4_cast(const aiMatrix4x4& m) 
{ 
	return glm::transpose(glm::make_mat4(&m.a1)); // Convert aiMatrix4x4 to glm::mat4
}

static inline glm::vec3 vec3_cast(const aiVector3D& v)
{
	return glm::vec3(v.x, v.y, v.z);
}

static inline glm::quat quat_cast(const aiQuaternion& q)
{
	return glm::quat(q.w, q.x, q.y, q.z);
}

static int BuildBoneHierarchy(aiNode* node, std::vector<Bone>& bones, const std::unordered_map<std::string, uint32_t>& boneIndexLookUp)
{
	auto it = boneIndexLookUp.find(node->mName.C_Str()); // Find by name, possible error when two nodes have the same name
	if (it != boneIndexLookUp.end())
	{
		for (int i = 0; i < node->mNumChildren; i++)
		{
			int childIdx = BuildBoneHierarchy(node->mChildren[i], bones, boneIndexLookUp);
			if (childIdx >= 0)
				bones[it->second].Children.push_back(childIdx);
		}

		return it->second;
	}

	// Not bone
	for (int i = 0; i < node->mNumChildren; i++)
	{
		BuildBoneHierarchy(node->mChildren[i], bones, boneIndexLookUp);
	}

	return -1;
}


Entity Model::LoadSkinned(Ref<Scene> scene, const std::string& filepath)
{
	CORE_TRACE("Loading skinned model, filepath = \"{0}\"", filepath);

	Assimp::Importer importer; 
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, 4);

	uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_LimitBoneWeights;
	const aiScene* fileScene = importer.ReadFile(AssetManager::GetFilepath(filepath).string(), flags);

	if (!fileScene)
	{
		CORE_ERROR("Failed to load skinned model, filepath = \"{0}\"\nError : {1}", filepath, importer.GetErrorString());
		return {};
	}

	Entity model = scene->CreateEntity(filepath);
	auto& meshRenderer = model.AddComponent<SkinnedMeshRendererComponent>();
	auto& materialTable = model.AddComponent<MaterialTableComponent>();
	auto& animator = model.AddComponent<AnimatorComponent>();

	CORE_TRACE("Skinned model has {0} submeshes, {1} materials, {2} animations", fileScene->mNumMeshes, fileScene->mNumMaterials, fileScene->mNumAnimations);


	// Generate mesh
	SkinnedMeshData meshData;
	meshData.Submeshes.resize(fileScene->mNumMeshes);


	// Submeshes
	uint32_t vertexCount = 0, indexCount = 0;
	for (uint32_t i = 0; i < fileScene->mNumMeshes; i++)
	{
		const aiMesh* submesh = fileScene->mMeshes[i];
		meshData.Submeshes[i].BaseVertex = vertexCount;
		meshData.Submeshes[i].BaseIndex = indexCount;
		meshData.Submeshes[i].IndexCount = submesh->mNumFaces * 3;
		meshData.Submeshes[i].MaterialIndex = submesh->mMaterialIndex;
		
		vertexCount += submesh->mNumVertices;
		indexCount += meshData.Submeshes[i].IndexCount;
	}

	
	// Vertex attributes
	meshData.Positions.reserve(vertexCount);
	meshData.Normals.reserve(vertexCount);
	meshData.TexCoords.reserve(vertexCount);
	meshData.Indices.reserve(indexCount);

	// Populate the buffers
	for (uint32_t i = 0; i < fileScene->mNumMeshes; i++)
	{
		const aiMesh* submesh = fileScene->mMeshes[i];

		// Vertex attributes
		for (uint32_t j = 0; j < submesh->mNumVertices; j++)
		{
			const aiVector3D& position = submesh->mVertices[j];
			const aiVector3D& normal = submesh->mNormals[j];
			const aiVector3D& texCoord = submesh->HasTextureCoords(0) ? submesh->mTextureCoords[0][j] : aiVector3D(0.f, 0.f, 0.f);

			meshData.Positions.emplace_back(position.x, position.y, position.z);
			meshData.Normals.emplace_back(normal.x, normal.y, normal.z);
			meshData.TexCoords.emplace_back(texCoord.x, texCoord.y);
		}

		// Index buffer
		for (uint32_t j = 0; j < submesh->mNumFaces; j++)
		{
			const aiFace& face = submesh->mFaces[j];

			meshData.Indices.emplace_back(face.mIndices[0]);
			meshData.Indices.emplace_back(face.mIndices[1]);
			meshData.Indices.emplace_back(face.mIndices[2]);
		}
	}


	// Vertex weights
	meshData.BoneIndex.resize(vertexCount, glm::ivec4(-1)); // -1 represent no bones
	meshData.BoneWeight.resize(vertexCount);
	std::vector<int> vertexBoneCount(vertexCount, 0);

	uint32_t vertexIdx, boneCount = 0;
	std::unordered_map<std::string, uint32_t> boneIndexLookUp;

	for (uint32_t i = 0; i < fileScene->mNumMeshes; i++)
	{
		const aiMesh* submesh = fileScene->mMeshes[i];
		for (uint32_t j = 0; j < submesh->mNumBones; j++)
		{
			const aiBone* bone = submesh->mBones[j];

			boneIndexLookUp[bone->mName.C_Str()] = boneCount;

			for (uint32_t k = 0; k < bone->mNumWeights; k++)
			{
				vertexIdx = meshData.Submeshes[i].BaseVertex + bone->mWeights[k].mVertexId;
				meshData.BoneIndex[vertexIdx][vertexBoneCount[vertexIdx]] = boneCount;
				meshData.BoneWeight[vertexIdx][vertexBoneCount[vertexIdx]] = bone->mWeights[k].mWeight;
				vertexBoneCount[vertexIdx]++;
			}

			boneCount++;
		}
	}


	// Skeleton
	std::vector<Bone> bones(boneCount);
	uint32_t boneIdx = 0;
	for (uint32_t i = 0; i < fileScene->mNumMeshes; i++)
	{
		const aiMesh* submesh = fileScene->mMeshes[i];
		for (uint32_t j = 0; j < submesh->mNumBones; j++, boneIdx++)
		{
			const aiBone* bone = submesh->mBones[j];
			bones[boneIdx].InverseBindMatrix = mat4_cast(submesh->mBones[j]->mOffsetMatrix);
		}
	}

	BuildBoneHierarchy(fileScene->mRootNode, bones, boneIndexLookUp);
	meshData.Skeleton = CreateRef<Skeleton>(bones);

	
	// Animations
	meshData.Animations.resize(fileScene->mNumAnimations);

	for (uint32_t i = 0; i < fileScene->mNumAnimations; i++)
	{
		aiAnimation* animation = fileScene->mAnimations[i];
				
		meshData.Animations[i] = CreateRef<Animation>();
		meshData.Animations[i]->Name = animation->mName.C_Str();
		meshData.Animations[i]->Length = animation->mDuration / (animation->mTicksPerSecond == 0 ? 0.25 : animation->mTicksPerSecond);
		std::vector<AnimationNode>& nodes = meshData.Animations[i]->Nodes;
		nodes.resize(boneCount);

		for (uint32_t j = 0; j < animation->mNumChannels; j++)
		{
			aiNodeAnim* channel = animation->mChannels[j];

			if (boneIndexLookUp.find(channel->mNodeName.C_Str()) == boneIndexLookUp.end())
				CORE_ERROR("Bone name {} not found", channel->mNodeName.C_Str());// TODO: remove?
			
			uint32_t boneIdx = boneIndexLookUp[channel->mNodeName.C_Str()];
			AnimationNode& node = nodes[boneIdx];

			node.KeyPositions.resize(channel->mNumPositionKeys);
			node.KeyRotations.resize(channel->mNumRotationKeys);
			node.KeyScales.resize(channel->mNumScalingKeys);


			for (uint32_t k = 0; k < channel->mNumPositionKeys; k++)
			{
				aiVectorKey& key = channel->mPositionKeys[k];
				node.KeyPositions[k].Position = vec3_cast(key.mValue);
				node.KeyPositions[k].TimeStamp = key.mTime / animation->mTicksPerSecond;
			}

			for (uint32_t k = 0; k < channel->mNumRotationKeys; k++)
			{
				aiQuatKey& key = channel->mRotationKeys[k];
				node.KeyRotations[k].Rotation = quat_cast(key.mValue);
				node.KeyRotations[k].TimeStamp = key.mTime / animation->mTicksPerSecond;
			}

			for (uint32_t k = 0; k < channel->mNumScalingKeys; k++)
			{
				aiVectorKey& key = channel->mScalingKeys[k];
				node.KeyScales[k].Scale = vec3_cast(key.mValue);
				node.KeyScales[k].TimeStamp = key.mTime / animation->mTicksPerSecond;
			}
		}
	}

	
	meshRenderer.SkinnedMesh = AssetManager::CreateSkinnedMesh(meshData);


	materialTable.Materials.resize(fileScene->mNumMaterials);

	for (int i = 0; i < fileScene->mNumMaterials; i++)
	{
		const aiMaterial* material = fileScene->mMaterials[i];

		AssetHandle albedoTexture = 0;
		AssetHandle normalTexture = 0;
		AssetHandle metallicTexture = 0;
		AssetHandle roughnessTexture = 0;
		AssetHandle ambientOcclusionTexture = 0;


		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{			
			aiString path;

			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				const aiTexture* embeddedTexture = fileScene->GetEmbeddedTexture(path.C_Str());

				if (embeddedTexture)
				{
					albedoTexture = AssetManager::CreateTexture((unsigned char*)embeddedTexture->pcData, embeddedTexture->mWidth, embeddedTexture->mHeight);
				}
				else
				{
					std::filesystem::path texturePath = std::filesystem::path(filepath).parent_path() / path.C_Str();
					albedoTexture = AssetManager::LoadTexture(texturePath.string());
				}
			}
			else
				CORE_ERROR("Failed to retrieve albedo texture path from ASSIMP");

		}
		
		if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
		{			
			aiString path;

			if (material->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS)
			{
				const aiTexture* embeddedTexture = fileScene->GetEmbeddedTexture(path.C_Str());

				if (embeddedTexture)
				{
					normalTexture = AssetManager::CreateTexture((unsigned char*)embeddedTexture->pcData, embeddedTexture->mWidth, embeddedTexture->mHeight);
				}
				else
				{
					std::filesystem::path texturePath = std::filesystem::path(filepath).parent_path() / path.C_Str();
					normalTexture = AssetManager::LoadTexture(texturePath.string());
				}
			}
			else
				CORE_ERROR("Failed to retrieve normal texture path from ASSIMP");

		}

		if (material->GetTextureCount(aiTextureType_METALNESS) > 0)
		{			
			aiString path;

			if (material->GetTexture(aiTextureType_METALNESS, 0, &path) == AI_SUCCESS)
			{
				const aiTexture* embeddedTexture = fileScene->GetEmbeddedTexture(path.C_Str());

				if (embeddedTexture)
				{
					metallicTexture = AssetManager::CreateTexture((unsigned char*)embeddedTexture->pcData, embeddedTexture->mWidth, embeddedTexture->mHeight);
				}
				else
				{
					std::filesystem::path texturePath = std::filesystem::path(filepath).parent_path() / path.C_Str();
					metallicTexture = AssetManager::LoadTexture(texturePath.string());
				}
			}
			else
				CORE_ERROR("Failed to retrieve metallic texture path from ASSIMP");

		}

		if (material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0)
		{			
			aiString path;

			if (material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &path) == AI_SUCCESS)
			{
				const aiTexture* embeddedTexture = fileScene->GetEmbeddedTexture(path.C_Str());

				if (embeddedTexture)
				{
					roughnessTexture = AssetManager::CreateTexture((unsigned char*)embeddedTexture->pcData, embeddedTexture->mWidth, embeddedTexture->mHeight);
				}
				else
				{
					std::filesystem::path texturePath = std::filesystem::path(filepath).parent_path() / path.C_Str();
					roughnessTexture = AssetManager::LoadTexture(texturePath.string());
				}
			}
			else
				CORE_ERROR("Failed to retrieve roughness texture path from ASSIMP");
		}
		
		if (material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) > 0)
		{			
			aiString path;

			if (material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &path) == AI_SUCCESS)
			{
				const aiTexture* embeddedTexture = fileScene->GetEmbeddedTexture(path.C_Str());

				if (embeddedTexture)
				{
					ambientOcclusionTexture = AssetManager::CreateTexture((unsigned char*)embeddedTexture->pcData, embeddedTexture->mWidth, embeddedTexture->mHeight);
				}
				else
				{
					std::filesystem::path texturePath = std::filesystem::path(filepath).parent_path() / path.C_Str();
					ambientOcclusionTexture = AssetManager::LoadTexture(texturePath.string());
				}
			}
			else
				CORE_ERROR("Failed to retrieve ambient occlusion texture path from ASSIMP");
		}

		CORE_TRACE("Material #{0} has {1} diffuse texture, {2} normal textures, {3} metallic texture, {4} roughness texture",
			i, material->GetTextureCount(aiTextureType_DIFFUSE), material->GetTextureCount(aiTextureType_NORMALS), material->GetTextureCount(aiTextureType_METALNESS), material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS));

		materialTable.Materials[i] = AssetManager::CreateMaterial(albedoTexture, normalTexture, metallicTexture, roughnessTexture, ambientOcclusionTexture);
	}

	importer.FreeScene();
	
	CORE_TRACE("Finished loading skinned model");

	return model;
}

Entity Model::LoadSkinned(Ref<Scene> scene, const std::string& filepath, const std::vector<AssetHandle>& materials)
{
	CORE_TRACE("Loading skinned model, filepath = \"{0}\"", filepath);

	Assimp::Importer importer; 
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, 4);

	uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_LimitBoneWeights;
	const aiScene* fileScene = importer.ReadFile(AssetManager::GetFilepath(filepath).string(), flags);

	if (!fileScene)
	{
		CORE_ERROR("Failed to load skinned model, filepath = \"{0}\"\nError : {1}", filepath, importer.GetErrorString());
		return {};
	}

	Entity model = scene->CreateEntity(filepath);
	auto& meshRenderer = model.AddComponent<SkinnedMeshRendererComponent>();
	auto& materialTable = model.AddComponent<MaterialTableComponent>();
	auto& animator = model.AddComponent<AnimatorComponent>();

	CORE_TRACE("Skinned model has {0} submeshes, {1} materials, {2} animations", fileScene->mNumMeshes, fileScene->mNumMaterials, fileScene->mNumAnimations);


	// Generate mesh
	SkinnedMeshData meshData;
	meshData.Submeshes.resize(fileScene->mNumMeshes);


	// Submeshes
	uint32_t vertexCount = 0, indexCount = 0;
	for (uint32_t i = 0; i < fileScene->mNumMeshes; i++)
	{
		const aiMesh* submesh = fileScene->mMeshes[i];
		meshData.Submeshes[i].BaseVertex = vertexCount;
		meshData.Submeshes[i].BaseIndex = indexCount;
		meshData.Submeshes[i].IndexCount = submesh->mNumFaces * 3;
		meshData.Submeshes[i].MaterialIndex = submesh->mMaterialIndex;
		
		vertexCount += submesh->mNumVertices;
		indexCount += meshData.Submeshes[i].IndexCount;
	}

	
	// Vertex attributes
	meshData.Positions.reserve(vertexCount);
	meshData.Normals.reserve(vertexCount);
	meshData.TexCoords.reserve(vertexCount);
	meshData.Indices.reserve(indexCount);

	// Populate the buffers
	for (uint32_t i = 0; i < fileScene->mNumMeshes; i++)
	{
		const aiMesh* submesh = fileScene->mMeshes[i];

		// Vertex attributes
		for (uint32_t j = 0; j < submesh->mNumVertices; j++)
		{
			const aiVector3D& position = submesh->mVertices[j];
			const aiVector3D& normal = submesh->mNormals[j];
			const aiVector3D& texCoord = submesh->HasTextureCoords(0) ? submesh->mTextureCoords[0][j] : aiVector3D(0.f, 0.f, 0.f);

			meshData.Positions.emplace_back(position.x, position.y, position.z);
			meshData.Normals.emplace_back(normal.x, normal.y, normal.z);
			meshData.TexCoords.emplace_back(texCoord.x, texCoord.y);
		}

		// Index buffer
		for (uint32_t j = 0; j < submesh->mNumFaces; j++)
		{
			const aiFace& face = submesh->mFaces[j];

			meshData.Indices.emplace_back(face.mIndices[0]);
			meshData.Indices.emplace_back(face.mIndices[1]);
			meshData.Indices.emplace_back(face.mIndices[2]);
		}
	}


	// Vertex weights
	meshData.BoneIndex.resize(vertexCount, glm::ivec4(-1)); // -1 represent no bones
	meshData.BoneWeight.resize(vertexCount);
	std::vector<int> vertexBoneCount(vertexCount, 0);

	uint32_t vertexIdx, boneCount = 0;
	std::unordered_map<std::string, uint32_t> boneIndexLookUp;

	for (uint32_t i = 0; i < fileScene->mNumMeshes; i++)
	{
		const aiMesh* submesh = fileScene->mMeshes[i];
		for (uint32_t j = 0; j < submesh->mNumBones; j++)
		{
			const aiBone* bone = submesh->mBones[j];

			boneIndexLookUp[bone->mName.C_Str()] = boneCount;

			for (uint32_t k = 0; k < bone->mNumWeights; k++)
			{
				vertexIdx = meshData.Submeshes[i].BaseVertex + bone->mWeights[k].mVertexId;
				meshData.BoneIndex[vertexIdx][vertexBoneCount[vertexIdx]] = boneCount;
				meshData.BoneWeight[vertexIdx][vertexBoneCount[vertexIdx]] = bone->mWeights[k].mWeight;
				vertexBoneCount[vertexIdx]++;
			}

			boneCount++;
		}
	}


	// Skeleton
	std::vector<Bone> bones(boneCount);
	uint32_t boneIdx = 0;
	for (uint32_t i = 0; i < fileScene->mNumMeshes; i++)
	{
		const aiMesh* submesh = fileScene->mMeshes[i];
		for (uint32_t j = 0; j < submesh->mNumBones; j++, boneIdx++)
		{
			const aiBone* bone = submesh->mBones[j];
			bones[boneIdx].InverseBindMatrix = mat4_cast(submesh->mBones[j]->mOffsetMatrix);
		}
	}

	BuildBoneHierarchy(fileScene->mRootNode, bones, boneIndexLookUp);
	meshData.Skeleton = CreateRef<Skeleton>(bones);

	
	// Animations
	meshData.Animations.resize(fileScene->mNumAnimations);

	for (uint32_t i = 0; i < fileScene->mNumAnimations; i++)
	{
		aiAnimation* animation = fileScene->mAnimations[i];
				
		meshData.Animations[i] = CreateRef<Animation>();
		meshData.Animations[i]->Name = animation->mName.C_Str();
		meshData.Animations[i]->Length = animation->mDuration / (animation->mTicksPerSecond == 0 ? 0.25 : animation->mTicksPerSecond);
		std::vector<AnimationNode>& nodes = meshData.Animations[i]->Nodes;
		nodes.resize(boneCount);

		for (uint32_t j = 0; j < animation->mNumChannels; j++)
		{
			aiNodeAnim* channel = animation->mChannels[j];

			if (boneIndexLookUp.find(channel->mNodeName.C_Str()) == boneIndexLookUp.end())
				CORE_ERROR("Bone name {} not found", channel->mNodeName.C_Str());// TODO: remove?
			
			uint32_t boneIdx = boneIndexLookUp[channel->mNodeName.C_Str()];
			AnimationNode& node = nodes[boneIdx];

			node.KeyPositions.resize(channel->mNumPositionKeys);
			node.KeyRotations.resize(channel->mNumRotationKeys);
			node.KeyScales.resize(channel->mNumScalingKeys);


			for (uint32_t k = 0; k < channel->mNumPositionKeys; k++)
			{
				aiVectorKey& key = channel->mPositionKeys[k];
				node.KeyPositions[k].Position = vec3_cast(key.mValue);
				node.KeyPositions[k].TimeStamp = key.mTime / animation->mTicksPerSecond;
			}

			for (uint32_t k = 0; k < channel->mNumRotationKeys; k++)
			{
				aiQuatKey& key = channel->mRotationKeys[k];
				node.KeyRotations[k].Rotation = quat_cast(key.mValue);
				node.KeyRotations[k].TimeStamp = key.mTime / animation->mTicksPerSecond;
			}

			for (uint32_t k = 0; k < channel->mNumScalingKeys; k++)
			{
				aiVectorKey& key = channel->mScalingKeys[k];
				node.KeyScales[k].Scale = vec3_cast(key.mValue);
				node.KeyScales[k].TimeStamp = key.mTime / animation->mTicksPerSecond;
			}
		}
	}

	
	meshRenderer.SkinnedMesh = AssetManager::CreateSkinnedMesh(meshData);


	CORE_VERIFY(materials.size(), "No material provided");


	materialTable.Materials.resize(fileScene->mNumMaterials);

	for (int i = 0; i < fileScene->mNumMaterials; i++)
	{
		if (i < materials.size())
			materialTable.Materials[i] = materials[i];
		else
		{
			materialTable.Materials[i] = materials.back();
			CORE_WARN("Skinned model requires {0} materials, only {1} materials were provided", fileScene->mNumMaterials, materials.size());
		}
	}
	
	importer.FreeScene();
	
	CORE_TRACE("Finished loading skinned model");

	return model;
}
