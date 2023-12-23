#pragma once

#include "Asset.h"
#include "Core/Base.h"
#include "Renderer/Texture.h"
#include "Renderer/Material.h"
#include "Renderer/Mesh.h"
#include "Renderer/SkinnedMesh.h"
#include "Renderer/Font.h"
#include "Audio/Audio.h"
#include "Animation/Animation.h"
#include <filesystem>
#include <unordered_map>


class AssetManager
{
public:
	AssetManager() = delete;

	static void SetAssetDirectory(const std::string& directory);

	static AssetHandle LoadTexture(const std::string& filepath);
	static AssetHandle CreateTexture(unsigned char* data, uint32_t width, uint32_t height);
	static AssetHandle CreateMaterial(AssetHandle albedoTexture, AssetHandle normalTexture = 0, AssetHandle metallicTexture = 0, AssetHandle roughnessTexture = 0, AssetHandle ambientOcclusionTexture = 0);
	static AssetHandle CreateMesh(const MeshData& meshData);
	static AssetHandle CreateSkinnedMesh(const SkinnedMeshData& meshData);
	static AssetHandle LoadFont(const std::string& filepath);
	static AssetHandle LoadMusic(const std::string& filepath);
	static AssetHandle LoadSound(const std::string& filepath);
	
	static void UnloadTexture(AssetHandle handle);
	static void UnloadMaterial(AssetHandle handle);
	static void UnloadMesh(AssetHandle handle);
	static void UnloadSkinnedMesh(AssetHandle handle);
	static void UnloadFont(AssetHandle handle);
	static void UnloadMusic(AssetHandle handle);
	static void UnloadSound(AssetHandle handle);

	static Ref<Texture2D> GetTexture(AssetHandle handle);
	static Ref<Material> GetMaterial(AssetHandle handle);
	static Ref<Mesh> GetMesh(AssetHandle handle);
	static Ref<SkinnedMesh> GetSkinnedMesh(AssetHandle handle);
	static Ref<Font> GetFont(AssetHandle handle);
	static Ref<Music> GetMusic(AssetHandle handle);
	static Ref<Sound> GetSound(AssetHandle handle);

	static std::filesystem::path GetFilepath(const std::string assetPath) { return s_AssetDirectory / assetPath; }

private:
	static std::filesystem::path s_AssetDirectory;
	static std::unordered_map<AssetHandle, Ref<Texture2D>> s_Textures;
	static std::unordered_map<AssetHandle, Ref<Material>> s_Materials;
	static std::unordered_map<AssetHandle, Ref<Mesh>> s_Meshes;
	static std::unordered_map<AssetHandle, Ref<SkinnedMesh>> s_SkinnedMeshes;
	static std::unordered_map<AssetHandle, Ref<Font>> s_Fonts;
	static std::unordered_map<AssetHandle, Ref<Music>> s_Musics;
	static std::unordered_map<AssetHandle, Ref<Sound>> s_Sounds;
};