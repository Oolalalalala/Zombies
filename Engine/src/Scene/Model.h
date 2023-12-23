#pragma once

#include "Scene.h"
#include "Asset/Asset.h"
#include <string>


class Model
{
public:
	Model() = delete;

	static Entity Load(Ref<Scene> scene, const std::string& filepath);
	static Entity Load(Ref<Scene> scene, const std::string& filepath, const std::vector<AssetHandle>& materials);

	static Entity LoadSkinned(Ref<Scene> scene, const std::string& filepath);
	static Entity LoadSkinned(Ref<Scene> scene, const std::string& filepath, const std::vector<AssetHandle>& materials);
};