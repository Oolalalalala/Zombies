#pragma once

#include "glm/glm.hpp"
#include "Core/Base.h"
#include "Texture.h"


struct Material
{
	Ref<Texture2D> AlbedoTexture;
	Ref<Texture2D> NormalTexture;
	Ref<Texture2D> MetallicTexture;
	Ref<Texture2D> RoughnessTexture;
	Ref<Texture2D> AmbientOcclusionTexture;
};