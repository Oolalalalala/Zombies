#pragma once

#include <GL/glew.h>
#include <unordered_map>

#include "Core/Base.h"
#include "VertexBufferLayout.h"
#include "glm/glm.hpp"



class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniformInt(const std::string& name, int value);
	void SetUniformIntArray(const std::string& name, int* value, uint32_t count);
	void SetUniformFloat(const std::string& name, float value);
	void SetUniformFloat2(const std::string& name, glm::vec2 value);
	void SetUniformFloat3(const std::string& name, glm::vec3 value);
	void SetUniformFloat4(const std::string& name, glm::vec4 value);
	void SetUniformMat4(const std::string& name, const glm::mat4& value);


	static Ref<Shader> Create(const std::string& filepath);

private:
	static GLenum StringToShaderType(const std::string& token);
	std::string ReadFile(const std::string& filepath);
	GLuint CreateShader(const std::string& source);
	GLuint CompileShader(GLenum type, const std::string& source);
	std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);

	int GetUniformLocation(const std::string& name);

private:
	uint32_t m_RendererID;
	std::string m_Filepath;

	std::unordered_map<std::string, uint32_t> m_UniformLocationCache;
};

