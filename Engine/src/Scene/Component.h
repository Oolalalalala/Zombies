#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

#include "Core/Base.h"
#include "Asset/AssetManager.h"
#include "Renderer/Camera.h"
#include "Core/IO.h"
#include <string>



struct TagComponent
{
	std::string Tag;
	TagComponent() = default;
	TagComponent(const std::string& tag)
		: Tag(tag) {}
};

struct TransformComponent
{
	glm::vec3 Position = glm::vec3(0.0f);
	glm::quat Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Identity
	glm::vec3 Scale = glm::vec3(1.0f);

	TransformComponent() = default;
	TransformComponent(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
		: Position(position), Rotation(rotation), Scale(scale)
	{
	}

	glm::mat4 GetTransform()
	{
		return glm::translate(glm::mat4(1.0f), Position)
			* glm::toMat4(Rotation)
			* glm::scale(glm::mat4(1.0f), Scale);
	}
};

struct CameraComponent
{
	::Camera Camera;
	bool Primary = true;
	bool Enabled = true;
	bool FixedAspectRatio = false;
	//RendererHandle RenderTarget = 0;

	CameraComponent() = default;
	CameraComponent(::Camera camera, /*RendererHandle renderTarget,*/ bool primary = true, bool enabled = true, bool fixedAspectRatio = false)
		: Camera(camera), /*RenderTarget(renderTarget),*/ Primary(primary), Enabled(enabled), FixedAspectRatio(fixedAspectRatio) {}
};


struct SpriteRendererComponent
{
	glm::vec2 Size = glm::vec2(10, 10);
	glm::vec4 Color = glm::vec4(1.0f);
	AssetHandle Texture = 0;
	bool Enabled = true;

	SpriteRendererComponent() = default;
	SpriteRendererComponent(glm::vec2 size, glm::vec4 color, AssetHandle texture = 0)
		: Size(size), Color(color), Texture(texture) {}
};

struct MeshRendererComponent
{
	AssetHandle Mesh = 0;
	bool Enabled = true;

	MeshRendererComponent() = default;
	MeshRendererComponent(AssetHandle mesh)
		: Mesh(mesh) {}
};

struct SkinnedMeshRendererComponent
{
	AssetHandle SkinnedMesh = 0;
	bool Enabled = true;
	
	SkinnedMeshRendererComponent() = default;
	SkinnedMeshRendererComponent(AssetHandle skinnedMesh)
		: SkinnedMesh(skinnedMesh) {}
};

struct MaterialTableComponent
{
	std::vector<AssetHandle> Materials;

	MaterialTableComponent() = default;
	MaterialTableComponent(std::initializer_list<AssetHandle> materials)
		: Materials(materials) {}
};

struct AnimatorComponent
{
	uint32_t AnimationIndex = 0;
	float AnimationTime = 0.0f;
	bool IsPlaying = false;
	
	AnimatorComponent() = default;
};

struct UIElementComponent
{
	glm::vec2 Anchor = glm::vec2(0.0f); // In scale  (-1 ~ 1)
	glm::vec2 Pivot = glm::vec2(0.0f);  // In scale  (-1 ~ 1)
	glm::vec2 Offset = glm::vec2(0.0f); // In pixels

	UIElementComponent() = default;
	UIElementComponent(glm::vec2 anchor, glm::vec2 pivot)
		: Anchor(anchor), Pivot(pivot) {}
};

struct TextRendererComponent
{
	std::string Text = "";
	AssetHandle Font = 0;
	float FontSize = 50.f;
	glm::vec2 Size = glm::vec2(400.0f, 200.0f); // In pixels
	glm::vec4 Color = glm::vec4(1.0f);
	glm::vec3 OutlineColor = glm::vec3(1.0f);
	float OutlineThickness = 0.0f;
	bool Enabled = true;

	TextRendererComponent() = default;
	TextRendererComponent(const std::string& text, AssetHandle font, float fontSize, glm::vec2 size, glm::vec4 color = glm::vec4(1.0f), glm::vec3 outlineColor = glm::vec3(1.0f), float outlineThickness = 0.0f)
		: Text(text), Font(font), FontSize(fontSize), Size(size), Color(color), OutlineColor(outlineColor), OutlineThickness(outlineThickness) {}
};

struct ButtonComponent // Attach to sprite renderer component
{
	MouseButton Button = MouseButton::Left;
	std::function<void()> ButtonDownCallback;
	bool Enabled = true;

	ButtonComponent() = default;
	ButtonComponent(MouseButton button, std::function<void()> callbackFunction)
		: Button(button), ButtonDownCallback(callbackFunction) {}
};

struct MusicSourceComponent
{
	AssetHandle Music;
	float Volume = 1.0f;
	float FadeIn = 0.0f;
	float FadeOut = 0.0f;
	bool Begin = false;
	bool Stop = false;

	MusicSourceComponent() = default;
	MusicSourceComponent(AssetHandle music, float volume = 1.0f, float fadeIn = 0.0f, float fadeOut = 0.0f)
		: Music(music), Volume(volume), FadeIn(fadeIn), FadeOut(fadeOut) {}
};

struct SoundSourceComponent
{
	AssetHandle Sound;
	float Volume = 1.0f;
	float FadeIn = 0.0f;
	float FadeOut = 0.0f;
	bool Begin = false;

	SoundSourceComponent() = default;
	SoundSourceComponent(AssetHandle sound, float volume = 1.0f, float fadeIn = 0.0f, float fadeOut = 0.0f)
		: Sound(sound), Volume(volume), FadeIn(fadeIn), FadeOut(fadeOut) {}
};