#include "pch.h"
#include "AssetManager.h"


uint64_t AssetHandle::s_Counter = 1; // Zero reserved for no texture

std::filesystem::path AssetManager::s_AssetDirectory;
std::unordered_map<AssetHandle, Ref<Texture2D>>     AssetManager::s_Textures;
std::unordered_map<AssetHandle, Ref<Material>>      AssetManager::s_Materials;
std::unordered_map<AssetHandle, Ref<Mesh>>          AssetManager::s_Meshes;
std::unordered_map<AssetHandle, Ref<SkinnedMesh>>   AssetManager::s_SkinnedMeshes;
std::unordered_map<AssetHandle, Ref<Font>>          AssetManager::s_Fonts;
std::unordered_map<AssetHandle, Ref<Music>>         AssetManager::s_Musics;
std::unordered_map<AssetHandle, Ref<Sound>>         AssetManager::s_Sounds;


void AssetManager::SetAssetDirectory(const std::string& directory)
{
	s_AssetDirectory = directory;
}

AssetHandle AssetManager::LoadTexture(const std::string& filepath)
{
	auto texture = Texture2D::Create((s_AssetDirectory / filepath).string());

	if (!texture)
		return 0;
	
	AssetHandle handle;
	s_Textures[handle] = texture;

	return handle;
}

AssetHandle AssetManager::CreateTexture(unsigned char* data, uint32_t width, uint32_t height)
{
	Texture2DSpecification specs;
	specs.Width = width;
	specs.Height = height;

	auto texture = Texture2D::Create(specs);
	texture->SetData(data);
	
	AssetHandle handle;
	s_Textures[handle] = texture;

	return handle;
}

AssetHandle AssetManager::CreateMaterial(AssetHandle albedoTexture, AssetHandle normalTexture, AssetHandle metallicTexture, AssetHandle roughnessTexture, AssetHandle ambientOcclusionTexture)
{
	auto material = CreateRef<Material>();

	if (albedoTexture)				material->AlbedoTexture			   = GetTexture(albedoTexture);
	if (normalTexture)				material->NormalTexture			   = GetTexture(normalTexture);
	if (metallicTexture)			material->MetallicTexture		   = GetTexture(metallicTexture);
	if (roughnessTexture)			material->RoughnessTexture		   = GetTexture(roughnessTexture);
	if (ambientOcclusionTexture)    material->AmbientOcclusionTexture  = GetTexture(ambientOcclusionTexture);
	
	AssetHandle handle;
	s_Materials[handle] = material;

	return handle;
}

AssetHandle AssetManager::CreateMesh(const MeshData& meshData)
{
	auto mesh = CreateRef<Mesh>(meshData);
	
	AssetHandle handle;
	s_Meshes[handle] = mesh;

	return handle;
}

AssetHandle AssetManager::CreateSkinnedMesh(const SkinnedMeshData& meshData)
{
	auto skinnedMesh = CreateRef<SkinnedMesh>(meshData);
	
	AssetHandle handle;
	s_SkinnedMeshes[handle] = skinnedMesh;

	return handle;
}

AssetHandle AssetManager::LoadFont(const std::string& filepath)
{
	auto font = CreateRef<Font>((s_AssetDirectory / filepath).string(), 50.f);

	AssetHandle handle;
	s_Fonts[handle] = font;
	
	return handle;
}

AssetHandle AssetManager::LoadMusic(const std::string& filepath)
{
	auto music = Music::Create((s_AssetDirectory / filepath).string());

	AssetHandle handle;
	s_Musics[handle] = music;
	
	return handle;
}

AssetHandle AssetManager::LoadSound(const std::string& filepath)
{
	auto sound = Sound::Create((s_AssetDirectory / filepath).string());

	AssetHandle handle;
	s_Sounds[handle] = sound;
	
	return handle;
}

// TODO : error handling for unload
void AssetManager::UnloadTexture(AssetHandle handle)
{
	s_Textures.erase(handle);
}

void AssetManager::UnloadMaterial(AssetHandle handle)
{
	s_Materials.erase(handle);
}

void AssetManager::UnloadMesh(AssetHandle handle)
{
	s_Meshes.erase(handle);
}

void AssetManager::UnloadSkinnedMesh(AssetHandle handle)
{
	s_SkinnedMeshes.erase(handle);
}

void AssetManager::UnloadFont(AssetHandle handle)
{
	s_Fonts.erase(handle);
}

void AssetManager::UnloadMusic(AssetHandle handle)
{
	s_Musics.erase(handle);
}

void AssetManager::UnloadSound(AssetHandle handle)
{
	s_Sounds.erase(handle);
}

Ref<Texture2D> AssetManager::GetTexture(AssetHandle handle)
{
	CORE_VERIFY(s_Textures.find(handle) != s_Textures.end(), "Invaliid AssetHandle to access Texture2D");

	return s_Textures[handle];
}

Ref<Material> AssetManager::GetMaterial(AssetHandle handle)
{
	CORE_VERIFY(s_Materials.find(handle) != s_Materials.end(), "Invaliid AssetHandle to access Material");

	return s_Materials[handle];
}

Ref<Mesh> AssetManager::GetMesh(AssetHandle handle)
{
	CORE_VERIFY(s_Meshes.find(handle) != s_Meshes.end(), "Invaliid AssetHandle to access Mesh");

	return s_Meshes[handle];
}

Ref<SkinnedMesh> AssetManager::GetSkinnedMesh(AssetHandle handle)
{
	CORE_VERIFY(s_SkinnedMeshes.find(handle) != s_SkinnedMeshes.end(), "Invaliid AssetHandle to access SkinnedMesh");

	return s_SkinnedMeshes[handle];
}

Ref<Font> AssetManager::GetFont(AssetHandle handle)
{
	CORE_VERIFY(s_Fonts.find(handle) != s_Fonts.end(), "Invaliid AssetHandle to access Font");

	return s_Fonts[handle];
}

Ref<Music> AssetManager::GetMusic(AssetHandle handle)
{
	CORE_VERIFY(s_Musics.find(handle) != s_Musics.end(), "Invaliid AssetHandle to access Music");

	return s_Musics[handle];
}

Ref<Sound> AssetManager::GetSound(AssetHandle handle)
{
	CORE_VERIFY(s_Sounds.find(handle) != s_Sounds.end(), "Invaliid AssetHandle to access Sound");

	return s_Sounds[handle];
}
