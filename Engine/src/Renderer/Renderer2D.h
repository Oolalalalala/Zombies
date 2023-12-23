#pragma once

#include <vector>

#include "Texture.h"
#include "Camera.h"
#include "Font.h"

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"



class Renderer2D
{
public:
	static void Initialize();
	static void ShutDown();

	static void BeginScene(const Camera& camera, glm::vec2 viewportSize);
	static void EndScene();

	// DrawQuad uses center point as origin
	static void DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color);                                         // 2D
	static void DrawQuad(glm::vec2 position, glm::vec2 size, Ref<Texture2D> texture, glm::vec4 color);                 // 2D
	static void DrawQuad(glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color);                         // 2D
	static void DrawQuad(glm::vec2 position, glm::vec2 size, float rotation, Ref<Texture2D> texture, glm::vec4 color); // 2D
	static void DrawQuad(glm::vec2 size, const glm::mat4& transform, glm::vec4 color);                                 // 3D
	static void DrawQuad(glm::vec2 size, const glm::mat4& transform, Ref<Texture2D> texture, glm::vec4 color);         // 3D

	// DrawString uses bottom-left point as origin
	static void DrawString(const std::string& text, Ref<Font> font, float fontSize, glm::vec2 position, glm::vec4 color = glm::vec4(1.0f)); // Position on screen space
	static void DrawString(const std::string& text, Ref<Font> font, float fontSize, const glm::mat4& transform, glm::vec4 color = glm::vec4(1.0f)); // Position on screen space
	static void DrawString(const std::string& text, Ref<Font> font, float fontSize, float outlineThickness, glm::vec2 position, glm::vec4 color = glm::vec4(1.0f), glm::vec3 outlineColor = glm::vec3(1.0f)); // Position on screen space
	static void DrawString(const std::string& text, Ref<Font> font, float fontSize, float outlineThickness, const glm::mat4& transform, glm::vec4 color = glm::vec4(1.0f), glm::vec3 outlineColor = glm::vec3(1.0f)); // Position on screen space
	static void DrawString(const std::string& text, Ref<Font> font, float fontSize, glm::vec2 size, glm::vec2 alignment, glm::vec2 position, float rotation, glm::vec2 scale, glm::vec4 color = glm::vec4(1.0f), float outlineThickness = 0.0f, glm::vec3 outlineColor = glm::vec3(1.0f)); // Position on screen space


	static void DrawCircle(glm::vec2 position, float radius, float thickness = 0.05f, glm::vec4 color = glm::vec4(1.0f));
	static void DrawLine();

private:
	static void FlushQuads();

	static uint32_t BatchCircles();
	static void FlushCircles(uint32_t indexCount);

	static void FlushCharacters();

	static void Reset();
};

