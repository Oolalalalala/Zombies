#include "pch.h"
#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>


Ref<Shader> Shader::Create(const std::string& filepath)
{
	return CreateRef<Shader>(filepath);
}

Shader::Shader(const std::string& filepath)
	: m_Filepath(filepath)
{
	m_RendererID = CreateShader(ReadFile(filepath));
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

GLenum Shader::StringToShaderType(const std::string& token)
{
	if (token == "vertex")							return GL_VERTEX_SHADER;
	if (token == "fragment" || token == "pixel")	return GL_FRAGMENT_SHADER;
	if (token == "geometry")						return GL_GEOMETRY_SHADER;
	return GL_NONE; // temp
}

std::string Shader::ReadFile(const std::string& filepath)
{
	std::string result;
	std::ifstream in(filepath);
	if (in)
	{
		in.seekg(0, std::ios::end);
		size_t size = (size_t)in.tellg();
		if (size != -1)
		{
			result.resize(size);
			in.seekg(0, std::ios::beg);
			in.read(&result[0], size);
		}
		else
			CORE_ERROR("Cannot open shader file, path = \"{0}\"", filepath);
	}
	else
		CORE_ERROR("Cannot open shader file, path = \"{0}\"", filepath);

	return result;
}

GLuint Shader::CreateShader(const std::string& source)
{
	GLuint program = glCreateProgram();

	auto shaderSources = PreProcess(source);
	std::vector<GLuint> shaders;

	for (auto& [type, src] : shaderSources)
	{
		GLuint shader = CompileShader(type, src);
		glAttachShader(program, shader);
		shaders.push_back(shader);
	}

	glLinkProgram(program);
	glValidateProgram(program);

	for (auto& shader : shaders)
	{
		glDetachShader(program, shader);
		glDeleteShader(shader);
	}

	return program;
}

GLuint Shader::CompileShader(GLenum type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	// Check for error
	GLint result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(id, length, &length, message);
		CORE_ERROR("Shader compilation failed, path = \"{0}\"\nMessage = \"{1}\"", m_Filepath, message);
		delete[] message;
	}

	return id;
}

std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& source)
{
	std::unordered_map<GLenum, std::string> shaderSources;

	const char* typeToken = "#type";
	size_t typeTokenLength = strlen(typeToken);
	size_t pos = source.find(typeToken);

	while (pos != std::string::npos)
	{
		size_t eol = source.find_first_of("\r\n", pos);
		size_t begin = pos + typeTokenLength + 1;
		GLenum type = StringToShaderType(source.substr(begin, eol - begin));

		size_t nextLinePos = source.find_first_not_of("\r\n", eol);
		pos = source.find(typeToken, nextLinePos);

		shaderSources[type] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
	}
	return shaderSources;
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniformInt(const std::string& name, int value)
{
	int location = GetUniformLocation(name);
	glUniform1i(location, value);
}

void Shader::SetUniformIntArray(const std::string& name, int* value, uint32_t count)
{
	int location = GetUniformLocation(name);
	glUniform1iv(location, count, value);
}

void Shader::SetUniformFloat(const std::string& name, float value)
{
	int location = GetUniformLocation(name);
	glUniform1f(location, value);
}

void Shader::SetUniformFloat2(const std::string& name, glm::vec2 value)
{
	int location = GetUniformLocation(name);
	glUniform2f(location, value.x, value.y);
}

void Shader::SetUniformFloat3(const std::string& name, glm::vec3 value)
{
	int location = GetUniformLocation(name);
	glUniform3f(location, value.x, value.y, value.z);
}

void Shader::SetUniformFloat4(const std::string& name, glm::vec4 value)
{
	int location = GetUniformLocation(name);
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& value)
{
	int location = GetUniformLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

int Shader::GetUniformLocation(const std::string& name)
{
	auto it = m_UniformLocationCache.find(name);

	if (it != m_UniformLocationCache.end())
		return it->second;

	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1)
	{
		CORE_ERROR("Uniform \"{0}\" does not exist", name);
		return -1;
	}

	return m_UniformLocationCache[name] = location;
}

