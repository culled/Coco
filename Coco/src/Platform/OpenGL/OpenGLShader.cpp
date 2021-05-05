#include "ccpch.h"
#include "OpenGLShader.h"

#include <fstream>

#include "Core/App/Log.h"
#include "glm/gtc/type_ptr.hpp"

namespace Coco {
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		ASSERT_CORE(false, "Unknown shader type");
		return 0;
	}

	static Shader::ShaderUniformType GetUniformType(GLenum uniformType)
	{
		switch (uniformType)
		{
		case GL_FLOAT:			return Shader::ShaderUniformType::Float;
		case GL_INT:				return Shader::ShaderUniformType::Int;
		case GL_BOOL:				return Shader::ShaderUniformType::Bool;
		case GL_FLOAT_VEC2: return Shader::ShaderUniformType::Float2;
		case GL_FLOAT_VEC3: return Shader::ShaderUniformType::Float3;
		case GL_FLOAT_VEC4: return Shader::ShaderUniformType::Float4;
		case GL_FLOAT_MAT3: return Shader::ShaderUniformType::Mat3;
		case GL_FLOAT_MAT4: return Shader::ShaderUniformType::Mat4;
		case GL_SAMPLER_2D: return Shader::ShaderUniformType::Sampler2D;
		}

		ASSERT_CORE(false, "Unknown uniform type");
		return Shader::ShaderUniformType::None;
	}

	OpenGLShader::OpenGLShader(const std::string& filePath, const std::string& name)
	{
		std::string shaderSource = ReadFile(filePath);

		auto processedSources = PreProcess(shaderSource);

		Compile(processedSources);

		if (name.empty())
		{
			//Extracts the name from the file path
			auto lastSlash = filePath.find_last_of("/\\");
			lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
			auto lastDot = filePath.rfind('.');
			auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
			m_Name = filePath.substr(lastSlash, count);
		}
		else
		{
			m_Name = name;
		}
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertSource, const std::string& fragSource)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertSource;
		sources[GL_FRAGMENT_SHADER] = fragSource;

		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		if (m_ProgramId)
			glDeleteProgram(m_ProgramId);
	}

	std::string OpenGLShader::ReadFile(const std::string& filePath)
	{
		std::string result;
		std::ifstream in(filePath, std::ios::in | std::ios::binary);

		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);

			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			LOG_CORE_WARN("Could not open shader file: {0}", filePath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> sources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			ASSERT_CORE(eol != std::string::npos, "Syntax error");

			size_t begin = pos + typeTokenLength + 1;
			std::string typeText = source.substr(begin, eol - begin);

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			sources[ShaderTypeFromString(typeText)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return sources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();

		std::vector<GLenum> glShaderIDs;

		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;
			const GLchar* sourceCstr = source.c_str();

			GLuint shader = glCreateShader(type);

			glShaderSource(shader, 1, &sourceCstr, 0);
			glCompileShader(shader);

			GLint status = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

			if (status == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				LOG_CORE_ERROR("{0}", infoLog.data());
				ASSERT_CORE(false, "Shader failed to compile.");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		glLinkProgram(program);

		int status;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&status);

		if (status == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(m_ProgramId);

			for (auto shader : glShaderIDs)
			{
				glDeleteShader(shader);
			}

			LOG_CORE_ERROR("{0}", infoLog.data());
			ASSERT_CORE(false, "Shader failed to link.");

			return;
		}

		for (auto shader : glShaderIDs)
		{
			glDetachShader(program, shader);
			glDeleteShader(shader);
		}

		m_ProgramId = program;
	}

	void OpenGLShader::Bind()
	{
		glUseProgram(m_ProgramId);
	}

	void OpenGLShader::Unbind()
	{
		glUseProgram(0);
	}

	int32_t OpenGLShader::GetUniformLocation(const std::string& name)
	{
		auto it = m_UniformCache.find(name);

		if (it == m_UniformCache.end())
		{
			int32_t location = glGetUniformLocation(m_ProgramId, name.c_str());
			ASSERT_CORE(location > -1, "Invalid uniform location");

			m_UniformCache[name] = (uint32_t)location;
			return location;
		}
		else
		{
			return (*it).second;
		}
	}

	std::unordered_map<std::string, Shader::ShaderUniformType> OpenGLShader::GetUniforms()
	{
		std::unordered_map<std::string, ShaderUniformType> map;

		int uniformCount = 0;
		glGetProgramiv(m_ProgramId, GL_ACTIVE_UNIFORMS, &uniformCount);

		const GLsizei bufferSize = 32;
		GLchar name[bufferSize];
		GLsizei nameLength;
		GLint varSize;
		GLenum varType;

		for (int i = 0; i < uniformCount; i++)
		{
			glGetActiveUniform(m_ProgramId, (GLuint)i, bufferSize, &nameLength, &varSize, &varType, name);

			map[std::string(name, nameLength)] = GetUniformType(varType);
		}

		return map;
	}

	void OpenGLShader::SetMatrix4(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetMatrix3(const std::string& name, const glm::mat3& matrix)
	{
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetVector4(const std::string& name, const glm::vec4& vector)
	{
		glUniform4f(GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
	}

	void OpenGLShader::SetVector3(const std::string& name, const glm::vec3& vector)
	{
		glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z);
	}

	void OpenGLShader::SetVector2(const std::string& name, const glm::vec2& vector)
	{
		glUniform2f(GetUniformLocation(name), vector.x, vector.y);
	}

	void OpenGLShader::SetFloat(const std::string& name, const float& value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetInt(const std::string& name, const int& value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		glUniform1iv(GetUniformLocation(name), count, values);
	}
}