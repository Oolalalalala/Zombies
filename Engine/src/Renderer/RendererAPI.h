#pragma once

#include "VertexArray.h"
#include "Framebuffer.h"
#include "glm/glm.hpp"



class RendererAPI
{
public:
	static void Initialize();

	static void SetDepthTest(bool enable);
	static void SetBlend(bool enable);

	static void SetClearColor(const glm::vec4& color);
	static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

	static void Blit(Ref<Framebuffer> source, Ref<Framebuffer> destination, uint32_t srcLeft, uint32_t srcRight, uint32_t srcTop, uint32_t srcBottom, uint32_t destLeft, uint32_t destRight, uint32_t destTop, uint32_t destBottom);
	static void BlitToDefaultFramebuffer(Ref<Framebuffer> source, uint32_t srcLeft, uint32_t srcRight, uint32_t srcTop, uint32_t srcBottom, uint32_t destLeft, uint32_t destRight, uint32_t destTop, uint32_t destBottom);
	static void SetPixelUnpackAlignment(uint32_t alignment);

	static void SetRenderTarget(Ref<Framebuffer> renderTarget);
	static void SetRenderTargetDefaultFramebuffer();

	static void Clear();
	static void DrawIndexed(Ref<VertexArray> vertexArray, uint32_t indexCount = 0);
	static void DrawIndexedOffset(uint32_t indexCount, uint32_t baseIndex, uint32_t baseVertex);
};

