#include "pch.h"
#include "Scene.h"

#include "Entity.h"
#include "Component.h"
#include "Core/IO.h"
#include "Asset/AssetManager.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/RendererAPI.h"
#include "SceneRenderer.h"
#include "Renderer/Font.h"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Audio/AudioMixer.h"
#include "Core/Timer.h"



Scene::Scene(const std::string& name)
	: m_Name(name)
{
	m_Renderer = CreateRef<SceneRenderer>();
}


void Scene::OnUpdate(float dt)
{
	Camera mainCamera;
	glm::vec2 windowSize = IO::GetWindowSize();

	// Cameras
	{
		auto group = m_Registry.group<CameraComponent>(entt::get<TransformComponent>);
		for (auto e : group)
		{
			auto [transform, camera] = group.get<TransformComponent, CameraComponent>(e);
			camera.Camera.SetPosition(transform.Position);
			camera.Camera.SetDirection(glm::inverse(glm::toMat4(transform.Rotation)) * glm::vec4(0.f, 0.f, 1.f, 0.0f));

			if (!camera.FixedAspectRatio && IO::IsWindowResized())
			{
				if (camera.Camera.IsPerspective())
				{
					auto specs = camera.Camera.GetPerspectiveCameraSpecification();
					specs.AspectRatio = windowSize.x / windowSize.y;
					camera.Camera.SetPerspective(specs);
				}
				else
				{
					auto specs = camera.Camera.GetOrthographicCameraSpecification();
					specs.Width = specs.Height * windowSize.x / windowSize.y;
					camera.Camera.SetOrthographic(specs);
				}
			}

			if (camera.Enabled && camera.Primary)
				mainCamera = camera.Camera;
		}
	}
	
	
	
	
	// Lighting
	{

	}
	
	
	// Render starts
	// TODO: move to other place
	//static Ref<Framebuffer> fb = Framebuffer::Create({});
	//if (IO::IsWindowResized())
	//	fb->Resize(windowSize.x, windowSize.y);
	//
	//
	//fb->Bind();
	if (IO::IsWindowResized)
	{
		RendererAPI::SetViewport(0, 0, windowSize.x, windowSize.y);
	}
	RendererAPI::SetClearColor(glm::vec4(0.1f));
	RendererAPI::Clear();


	RendererAPI::SetBackFaceCulling(true);
	m_Renderer->BeginScene(mainCamera);

	// Meshes
	{
		auto group = m_Registry.group<MeshRendererComponent, MaterialTableComponent>(entt::get<TransformComponent>);

		for (auto e : group)
		{
			auto [transform, mesh, materialTable] = group.get<TransformComponent, MeshRendererComponent, MaterialTableComponent>(e);

			if (!mesh.Enabled)
				continue;

			static std::vector<Ref<Material>> materials;
			materials.clear();

			for (AssetHandle handle : materialTable.Materials)
				materials.emplace_back(AssetManager::GetMaterial(handle));

			m_Renderer->SubmitMesh(AssetManager::GetMesh(mesh.Mesh), materials, transform.GetTransform());
		}
	}

	// Skinned mesh
	{
		auto group = m_Registry.group<SkinnedMeshRendererComponent, AnimatorComponent>(entt::get<TransformComponent, MaterialTableComponent>);
		
		for (auto e : group)
		{
			auto [transform, mesh, materialTable, animator] = group.get<TransformComponent, SkinnedMeshRendererComponent, MaterialTableComponent, AnimatorComponent>(e);
			
			if (!mesh.Enabled)
				continue;

			std::vector<Ref<Material>> materials;
			for (AssetHandle handle : materialTable.Materials)
				materials.emplace_back(AssetManager::GetMaterial(handle));

			Ref<SkinnedMesh> skinnedMesh = AssetManager::GetSkinnedMesh(mesh.SkinnedMesh);
			Ref<Animation> animation = skinnedMesh->GetAnimation(animator.AnimationIndex);
			
			m_Renderer->SubmitSkinnedMesh(skinnedMesh, materials, transform.GetTransform(), animation, animator.AnimationTime);

			if (animator.IsPlaying)
				animator.AnimationTime += dt;
		}
	}

	m_Renderer->EndScene();

	
	
	RendererAPI::SetBackFaceCulling(false);
	Renderer2D::BeginScene(mainCamera, windowSize);


	// 3D Sprites
	{
		auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>(entt::exclude<UIElementComponent>);

		for (auto e : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(e);
			
			if (!sprite.Enabled)
				continue;

			if (sprite.Texture)
				Renderer2D::DrawQuad(sprite.Size, transform.GetTransform(), AssetManager::GetTexture(sprite.Texture), sprite.Color);
			else
				Renderer2D::DrawQuad(sprite.Size, transform.GetTransform(), sprite.Color);
		}

	}


	// 2D Sprites
	{
		auto group = m_Registry.group<UIElementComponent>(entt::get<TransformComponent, SpriteRendererComponent>);

		for (auto e : group)
		{
			auto [transform, sprite, UI] = group.get<TransformComponent, SpriteRendererComponent, UIElementComponent>(e);
			
			glm::vec2 anchor, center, position;

			if (sprite.Enabled)
			{
				anchor = UI.Anchor * windowSize * 0.5f;
				center = anchor + UI.Offset;
				float roll = -glm::roll(transform.Rotation);
				position = center - glm::rotate(sprite.Size * glm::vec2(transform.Scale) * UI.Pivot * 0.5f, roll);

				if (sprite.Texture)
					Renderer2D::DrawQuad(position, sprite.Size, roll, AssetManager::GetTexture(sprite.Texture), sprite.Color);
				else
					Renderer2D::DrawQuad(position, sprite.Size, roll, sprite.Color);
			}

			// Check button
			Entity entity(e, this);
			if (entity.HasComponent<ButtonComponent>())
			{
				auto& button = entity.GetComponent<ButtonComponent>();

				if (!button.Enabled || !button.ButtonDownCallback || !IO::IsMouseButtonDown(button.Button))
					continue;

				glm::vec2 mousePos = IO::GetMousePosition() - windowSize * 0.5f;
				mousePos.y = -mousePos.y;
				glm::vec2 offset = glm::abs(mousePos - position) * 2.0f;

				if (offset.x <= sprite.Size.x && offset.y <= sprite.Size.y)
					button.ButtonDownCallback();
			}
		}

	}


	// 2D Text
	{
		auto group = m_Registry.group<TextRendererComponent>(entt::get<TransformComponent, UIElementComponent>);

		for (auto e : group)
		{
			auto [transform, text, UI] = group.get<TransformComponent, TextRendererComponent, UIElementComponent>(e);

			if (!text.Enabled)
				continue;

			glm::vec2 anchor = UI.Anchor * windowSize * 0.5f;
			glm::vec2 center = anchor + UI.Offset;
			float roll = -glm::roll(transform.Rotation);
			glm::vec2 position = center - glm::rotate(text.Size * glm::vec2(transform.Scale) * (UI.Pivot + glm::vec2(1.0f)) * 0.5f, roll);

			glm::mat4 tf = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f))
						 * glm::rotate(glm::mat4(1.0f), roll, glm::vec3(0, 0, 1))
						 * glm::scale(glm::mat4(1.0f), glm::vec3(transform.Scale));

			Renderer2D::DrawString(text.Text, AssetManager::GetFont(text.Font), text.FontSize, text.OutlineThickness, tf, text.Color, text.OutlineColor);
			//position = center - text.Size * glm::vec2(transform.Scale) * UI.Pivot * 0.5f;
			//Renderer2D::DrawString(text.Text, AssetManager::GetFont(text.Font), text.FontSize, text.Size, text.Alignment, position, roll, glm::vec2(transform.Scale), text.Color, text.OutlineThickness, text.OutlineColor);
		}

	}

	Renderer2D::EndScene();


	//RendererAPI::BlitToDefaultFramebuffer(fb, 0, windowSize.x, windowSize.y, 0, 0, windowSize.x, windowSize.y, 0);


	// Music
	{
		auto view = m_Registry.view<MusicSourceComponent>();

		for (auto e : view)
		{
			auto& music = view.get<MusicSourceComponent>(e);
			if (music.Begin)
			{
				AudioMixer::PlayMusic(AssetManager::GetMusic(music.Music), music.Volume, music.FadeIn);
				music.Begin = false;
			}
			if (music.Stop)
			{
				// TODO : check is playing
				AudioMixer::StopMusic(AssetManager::GetMusic(music.Music), music.FadeOut);
				music.Stop = false;
			}
		}
	}
	
	// Sound
	{
		auto view = m_Registry.view<SoundSourceComponent>();

		for (auto e : view)
		{
			auto& sound = view.get<SoundSourceComponent>(e);
			if (sound.Begin)
			{
				AudioMixer::PlaySound(AssetManager::GetSound(sound.Sound), sound.Volume, sound.FadeIn);
				sound.Begin = false;
			}
		}
	}
}

Entity Scene::CreateEntity(const std::string& name)
{
	Entity entity(m_Registry.create(), this);
	entity.AddComponent<TagComponent>(name.empty() ? "Entity" : name);
	entity.AddComponent<TransformComponent>();
	return entity;
}

Entity Scene::CreateEntity(Entity entity, const std::string& name)
{
	Entity newEntity(m_Registry.create(), this);

	// Tag
	{
		auto& newTag = newEntity.AddComponent<TagComponent>();
		auto& oldTag = entity.GetComponent<TagComponent>();
		newTag.Tag = name.empty() ? oldTag.Tag : name;
	}
	
	// Transform
	{
		auto& newTransform = newEntity.AddComponent<TransformComponent>();
		auto& oldTransform = entity.GetComponent<TransformComponent>();
		newTransform = oldTransform;
	}

	if (entity.HasComponent<CameraComponent>())
	{
		auto& newCamera = newEntity.AddComponent<CameraComponent>();
		auto& oldCamera = entity.GetComponent<CameraComponent>();
		newCamera = oldCamera;
	}

	if (entity.HasComponent<SpriteRendererComponent>())
	{
		auto& newSprite = newEntity.AddComponent<SpriteRendererComponent>();
		auto& oldSprite = entity.GetComponent<SpriteRendererComponent>();
		newSprite = oldSprite;
	}

	if (entity.HasComponent<MeshRendererComponent>())
	{
		auto& newMesh = newEntity.AddComponent<MeshRendererComponent>();
		auto& oldMesh = entity.GetComponent<MeshRendererComponent>();
		newMesh = oldMesh;
	}
	
	if (entity.HasComponent<SkinnedMeshRendererComponent>())
	{
		auto& newMesh = newEntity.AddComponent<SkinnedMeshRendererComponent>();
		auto& oldMesh = entity.GetComponent<SkinnedMeshRendererComponent>();
		newMesh = oldMesh;
	}

	if (entity.HasComponent<MaterialTableComponent>())
	{
		auto& newMaterial = newEntity.AddComponent<MaterialTableComponent>();
		auto& oldMaterial = entity.GetComponent<MaterialTableComponent>();
		newMaterial = oldMaterial;
	}
	
	if (entity.HasComponent<UIElementComponent>())
	{
		auto& newUI = newEntity.AddComponent<UIElementComponent>();
		auto& oldUI = entity.GetComponent<UIElementComponent>();
		newUI = oldUI;
	}

	if (entity.HasComponent<TextRendererComponent>())
	{
		auto& newText = newEntity.AddComponent<TextRendererComponent>();
		auto& oldText = entity.GetComponent<TextRendererComponent>();
		newText = oldText;
	}
	
	if (entity.HasComponent<ButtonComponent>())
	{
		auto& newButton = newEntity.AddComponent<ButtonComponent>();
		auto& oldButton = entity.GetComponent<ButtonComponent>();
		newButton = oldButton;
	}

	if (entity.HasComponent<AnimatorComponent>())
	{
		auto& newAnimator = newEntity.AddComponent<AnimatorComponent>();
		auto& oldAnimator = entity.GetComponent<AnimatorComponent>();
		newAnimator = oldAnimator;
	}

	return newEntity;
}

void Scene::DestroyEntity(Entity entity)
{
	m_Registry.destroy(entity.m_EntityID);
}

Ref<Scene> Scene::Copy(Ref<Scene> source)
{
	Ref<Scene> newScene = CreateRef<Scene>();

	newScene->m_Name = source->m_Name;

	auto tagView = source->m_Registry.view<TagComponent>();
	for (auto e : tagView)
	{
		Entity newEntity = newScene->CreateEntity(tagView.get<TagComponent>(e).Tag);
		Entity oldEntity(e, source.get());

		// TODO
	}

	return newScene;
}