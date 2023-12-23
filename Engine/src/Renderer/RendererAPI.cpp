#include "pch.h"
#include "RendererAPI.h"



void RendererAPI::Initialize()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
}

void RendererAPI::SetDepthTest(bool enable)
{
	if (enable)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void RendererAPI::SetBlend(bool enable)
{
	if (enable)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
		glDisable(GL_BLEND);
}

void RendererAPI::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void RendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	glViewport(x, y, width, height);
}

void RendererAPI::Blit(Ref<Framebuffer> source, Ref<Framebuffer> destination, uint32_t srcLeft, uint32_t srcRight, uint32_t srcTop, uint32_t srcBottom, uint32_t destLeft, uint32_t destRight, uint32_t destTop, uint32_t destBottom)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, source->m_RendererID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destination->m_RendererID);
	glBlitFramebuffer(srcLeft, srcBottom, srcRight, srcTop, destLeft, destBottom, destRight, destTop, GL_COLOR_BUFFER_BIT, GL_LINEAR); // TDOO : Add options
}

void RendererAPI::BlitToDefaultFramebuffer(Ref<Framebuffer> source, uint32_t srcLeft, uint32_t srcRight, uint32_t srcTop, uint32_t srcBottom, uint32_t destLeft, uint32_t destRight, uint32_t destTop, uint32_t destBottom)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, source->m_RendererID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(srcLeft, srcBottom, srcRight, srcTop, destLeft, destBottom, destRight, destTop, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void RendererAPI::SetPixelUnpackAlignment(uint32_t alignment)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
}

void RendererAPI::SetRenderTarget(Ref<Framebuffer> renderTarget)
{
	renderTarget->Bind();
}

void RendererAPI::SetRenderTargetDefaultFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RendererAPI::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererAPI::DrawIndexed(Ref<VertexArray> vertexArray, uint32_t indexCount)
{
	vertexArray->Bind();
	uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void RendererAPI::DrawIndexedOffset(uint32_t indexCount, uint32_t baseIndex, uint32_t baseVertex)
{
	glDrawElementsBaseVertex(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * baseIndex), baseVertex);
}

