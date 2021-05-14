#include "ccpch.h"
#include "OpenGLShader.h"

#include "Core/App/Log.h"

#include <fstream>
#include <filesystem>

#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace Coco {
	std::string OpenGLShader::s_CacheDirectory = "assets/cache/shaders/opengl";

#pragma region Helpers
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		ASSERT_CORE(false, "Unknown shader type");
		return 0;
	}

	static void CreateShaderCacheDirectoryIfNeeded()
	{
		if (!std::filesystem::exists(OpenGLShader::s_CacheDirectory))
			std::filesystem::create_directories(OpenGLShader::s_CacheDirectory);
	}

	static ShaderDataType GetUniformType(GLenum uniformType)
	{
		switch (uniformType)
		{
		case GL_FLOAT:			return ShaderDataType::Float;
		case GL_INT:				return ShaderDataType::Int;
		case GL_BOOL:				return ShaderDataType::Bool;
		case GL_FLOAT_VEC2: return ShaderDataType::Float2;
		case GL_FLOAT_VEC3: return ShaderDataType::Float3;
		case GL_FLOAT_VEC4: return ShaderDataType::Float4;
		case GL_FLOAT_MAT3: return ShaderDataType::Mat3;
		case GL_FLOAT_MAT4: return ShaderDataType::Mat4;
		}

		ASSERT_CORE(false, "Unknown uniform type");
		return ShaderDataType::None;
	}

	static const char* GetCachedOpenGLShaderExtension(GLenum type)
	{
		switch (type)
		{
		case GL_VERTEX_SHADER: return ".opengl.vert.cache";
		case GL_FRAGMENT_SHADER: return ".opengl.frag.cache";
		}

		ASSERT_CORE(false, "Invalid type");
		return "";
	}

	static const char* GetCachedVulkanShaderExtension(GLenum type)
	{
		switch (type)
		{
		case GL_VERTEX_SHADER: return ".vulkan.vert.cache";
		case GL_FRAGMENT_SHADER: return ".vulkan.frag.cache";
		}

		ASSERT_CORE(false, "Invalid type");
		return "";
	}

	static shaderc_shader_kind ShaderTypeToShaderC(GLenum type)
	{
		switch (type)
		{
		case GL_VERTEX_SHADER: return shaderc_glsl_vertex_shader;
		case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
		}

		ASSERT_CORE(false, "Invalid type");
		return (shaderc_shader_kind)0;
	}

	static const char* ShaderTypeToString(GLenum type)
	{
		switch (type)
		{
		case GL_VERTEX_SHADER: return "Vertex";
		case GL_FRAGMENT_SHADER: return "Fragment";
		}

		ASSERT_CORE(false, "Invalid type");
		return "";
	}
#pragma endregion

	OpenGLShader::OpenGLShader(const std::string& filePath, const std::string& name)
		: m_FilePath(filePath)
	{
		CreateShaderCacheDirectoryIfNeeded();

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

		std::string shaderSource = ReadFile(filePath);
		auto processedSources = PreProcess(shaderSource);

		CompileOrGetVulkanBinaries(processedSources);
		CompileOrGetOpenGLBinaries();

		CreateProgram();

	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertSource, const std::string& fragSource)
		: m_Name(name), m_FilePath("")
	{
		ShaderMap sources;
		sources[GL_VERTEX_SHADER] = vertSource;
		sources[GL_FRAGMENT_SHADER] = fragSource;

		CompileOrGetVulkanBinaries(sources);
		CompileOrGetOpenGLBinaries();

		CreateProgram();
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

	OpenGLShader::ShaderMap OpenGLShader::PreProcess(const std::string& source)
	{
		ShaderMap sources;

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

	/*void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
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
	}*/

#pragma region Creation
	void OpenGLShader::CompileOrGetVulkanBinaries(const ShaderMap& shaderSources)
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions compileOptions;

		compileOptions.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		compileOptions.SetOptimizationLevel(shaderc_optimization_level_performance);
		compileOptions.SetGenerateDebugInfo();

		std::filesystem::path cacheDirectory = s_CacheDirectory;

		auto& shaderData = m_VulkanSpirV;
		shaderData.clear();

		for (auto&& [type, source] : shaderSources)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + GetCachedVulkanShaderExtension(type));

			std::ifstream file(cachedPath, std::ios::in | std::ios::binary);
			if (file.is_open())
			{
				file.seekg(0, std::ios::end);
				auto fileSize = file.tellg();
				file.seekg(0, std::ios::beg);

				auto& data = shaderData[type];
				data.resize(fileSize / sizeof(uint32_t));
				file.read((char*)data.data(), fileSize);
				file.close();
			}
			else
			{
				shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(source, ShaderTypeToShaderC(type), m_FilePath.c_str(), compileOptions);

				if (result.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					LOG_CORE_ERROR("Error compiling {0} to Vulkan: {1}", ShaderTypeToString(type), result.GetErrorMessage());
					ASSERT_CORE(false, result.GetErrorMessage());
				}

				shaderData[type] = std::vector<uint32_t>(result.cbegin(), result.cend());

				std::ofstream outFile(cachedPath, std::ios::out | std::ios::binary);
				if (outFile.is_open())
				{
					auto& data = shaderData[type];
					outFile.write((char*)data.data(), data.size() * sizeof(uint32_t));
					outFile.flush();
					outFile.close();
				}
				else
				{
					ASSERT_CORE(false, "Unable to open output file");
				}
			}
		}
	}

	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions compileOptions;

		compileOptions.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		compileOptions.SetOptimizationLevel(shaderc_optimization_level_performance);
		compileOptions.SetAutoMapLocations(true);
		compileOptions.SetAutoBindUniforms(true);
		compileOptions.SetGenerateDebugInfo();

		std::filesystem::path cacheDirectory = s_CacheDirectory;

		auto& shaderData = m_OpenGLSpirV;
		shaderData.clear();
		m_OpenGLSourceCode.clear();

		for (auto&& [type, spirV] : m_VulkanSpirV)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + GetCachedOpenGLShaderExtension(type));

			std::ifstream file(cachedPath, std::ios::in | std::ios::binary);
			if (file.is_open())
			{
				file.seekg(0, std::ios::end);
				auto fileSize = file.tellg();
				file.seekg(0, std::ios::beg);

				auto& data = shaderData[type];
				data.resize(fileSize / sizeof(uint32_t));
				file.read((char*)data.data(), fileSize);
				file.close();
			}
			else
			{
				spirv_cross::CompilerGLSL glslCompiler(spirV);
				m_OpenGLSourceCode[type] = glslCompiler.compile();
				auto& source = m_OpenGLSourceCode[type];

				shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(source, ShaderTypeToShaderC(type), m_FilePath.c_str(), compileOptions);

				if (result.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					LOG_CORE_ERROR("Error compiling {0} to OpenGL: {1}", ShaderTypeToString(type), result.GetErrorMessage());
					ASSERT_CORE(false, result.GetErrorMessage());
				}

				shaderData[type] = std::vector<uint32_t>(result.cbegin(), result.cend());

				std::ofstream outFile(cachedPath, std::ios::out | std::ios::binary);
				if (outFile.is_open())
				{
					auto& data = shaderData[type];
					outFile.write((char*)data.data(), data.size() * sizeof(uint32_t));
					outFile.flush();
					outFile.close();
				}
				else
				{
					ASSERT_CORE(false, "Unable to open output file");
				}
			}
		}
	}

	void OpenGLShader::CreateProgram()
	{
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIds;

		for (auto&& [type, spirV] : m_OpenGLSpirV)
		{
			GLuint shaderId = shaderIds.emplace_back(glCreateShader(type));
			glShaderBinary(1, &shaderId, GL_SHADER_BINARY_FORMAT_SPIR_V, spirV.data(), spirV.size() * sizeof(uint32_t));
			glSpecializeShader(shaderId, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderId);
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

			for (auto shader : shaderIds)
			{
				glDeleteShader(shader);
			}

			LOG_CORE_ERROR("{0}", infoLog.data());
			ASSERT_CORE(false, "Shader failed to link.");

			return;
		}

		for (auto shader : shaderIds)
		{
			glDetachShader(program, shader);
			glDeleteShader(shader);
		}

		m_ProgramId = program;

		for (auto&& [type, data] : m_VulkanSpirV)
		{
			//Reflect(type, data);
			GetUniforms(type, data);
		}
	}

	void OpenGLShader::Reflect(GLenum type, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::CompilerGLSL compiler(shaderData);
		auto active = compiler.get_active_interface_variables();
		spirv_cross::ShaderResources resources = compiler.get_shader_resources(active);

		LOG_CORE_TRACE("Reflecting shader {0} ({1}):", m_Name, ShaderTypeToString(type));
		LOG_CORE_TRACE("	- {0} samplers", resources.sampled_images.size());
		LOG_CORE_TRACE("	- {0} uniform buffers:", resources.uniform_buffers.size());

		for (const auto& buffer : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(buffer.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t bindingLocation = compiler.get_decoration(buffer.id, spv::DecorationBinding);

			int memberCount = bufferType.member_types.size();

			LOG_CORE_TRACE("		- {0}:", buffer.name);
			LOG_CORE_TRACE("			- Size = {0}", bufferSize);
			LOG_CORE_TRACE("			- Location = {0}", bindingLocation);
			LOG_CORE_TRACE("			- Members = {0}:", memberCount);

			for (int i = 0; i < memberCount; i++)
			{
				LOG_CORE_TRACE("				- {0}", compiler.get_member_name(bufferType.self, i));
			}
		}

		LOG_CORE_TRACE("	- {0} push constants:", resources.push_constant_buffers.size());

		for (const auto& constant : resources.push_constant_buffers)
		{
			const auto& bufferType = compiler.get_type(constant.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t bindingLocation = compiler.get_decoration(constant.id, spv::DecorationLocation);
			int memberCount = bufferType.member_types.size();

			LOG_CORE_TRACE("		- {0}:", constant.name);
			LOG_CORE_TRACE("			- Size = {0}", bufferSize);
			LOG_CORE_TRACE("			- Location = {0}", bindingLocation);
			LOG_CORE_TRACE("			- Members = {0}:", memberCount);

			for (int i = 0; i < memberCount; i++)
			{
				LOG_CORE_TRACE("				- {0}:", compiler.get_member_name(bufferType.self, i));
				LOG_CORE_TRACE("					- Type = {0}", compiler.get_type(bufferType.member_types[i]).self);
			}
		}
	}

	void OpenGLShader::GetUniforms(GLenum type, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::CompilerGLSL compiler(shaderData);
		auto active = compiler.get_active_interface_variables();
		spirv_cross::ShaderResources resources = compiler.get_shader_resources(active);

		LOG_CORE_TRACE("Shader \"{0}\":", m_Name);
		LOG_CORE_TRACE(" - {0} push constants:", resources.push_constant_buffers.size());

		for (const auto& constant : resources.push_constant_buffers)
		{
			const auto& bufferType = compiler.get_type(constant.base_type_id);
			int memberCount = bufferType.member_types.size();

			LOG_CORE_TRACE("	- {0}:", constant.name);
			LOG_CORE_TRACE("		- {0} Members:", memberCount);

			for (int i = 0; i < memberCount; i++)
			{
				std::string memberName = constant.name + "." + compiler.get_member_name(bufferType.self, i);
				LOG_CORE_TRACE("			- {0}:", memberName);

				ShaderDataType uniformType = ShaderDataType::None;
				uint32_t location = 0;

				const auto& type = compiler.get_type(bufferType.member_types[i]);

				switch (type.columns)
				{
				case 1:
					switch (type.vecsize)
					{
					case 1:
						switch (type.basetype)
						{
						case spirv_cross::SPIRType::BaseType::Int:
							uniformType = ShaderDataType::Int;
							break;
						case spirv_cross::SPIRType::BaseType::Float:
							uniformType = ShaderDataType::Float;
							break;
						}
						break;
					case 2:
						switch (type.basetype)
						{
						case spirv_cross::SPIRType::BaseType::Int:
							uniformType = ShaderDataType::Int2;
							break;
						case spirv_cross::SPIRType::BaseType::Float:
							uniformType = ShaderDataType::Float2;
							break;
						}
						break;
					case 3:
						switch (type.basetype)
						{
						case spirv_cross::SPIRType::BaseType::Int:
							uniformType = ShaderDataType::Int3;
							break;
						case spirv_cross::SPIRType::BaseType::Float:
							uniformType = ShaderDataType::Float3;
							break;
						}
						break;
					case 4:
						switch (type.basetype)
						{
						case spirv_cross::SPIRType::BaseType::Int:
							uniformType = ShaderDataType::Int4;
							break;
						case spirv_cross::SPIRType::BaseType::Float:
							uniformType = ShaderDataType::Float4;
							break;
						}
						break;
					}
					break;
				case 3:
					uniformType = ShaderDataType::Mat3;
					break;
				case 4:
					uniformType = ShaderDataType::Mat4;
					break;
				}

				ASSERT_CORE(uniformType != ShaderDataType::None, "Invalid uniform type");

				m_Uniforms.emplace_back(memberName, uniformType, GetUniformLocation(memberName));
			}
		}

		LOG_CORE_TRACE(" - {0} samplers:", resources.sampled_images.size());

		for (const auto& sampler : resources.sampled_images)
		{
			std::string memberName = sampler.name;
			auto location = compiler.get_decoration(sampler.id, spv::Decoration::DecorationBinding);
			LOG_CORE_TRACE("	- {0} at {1}", sampler.name, location);

			m_Uniforms.emplace_back(memberName, ShaderDataType::Sampler2D, location);
		}
	}
#pragma endregion

#pragma region Shader runtime
	void OpenGLShader::Bind()
	{
		glUseProgram(m_ProgramId);
	}

	void OpenGLShader::Unbind()
	{
		glUseProgram(0);
	}

	void OpenGLShader::BindBuffer(const Ref<UniformBuffer>& buf, uint32_t location)
	{
		glUniformBlockBinding(m_ProgramId, buf->GetID(), location);
	}

	int32_t OpenGLShader::GetUniformLocation(const std::string& name)
	{
		std::vector<ShaderUniform>::iterator it = std::find_if(m_Uniforms.begin(), m_Uniforms.end(), [&](const auto& uniform) {
			return uniform.Name == name;
			});

		if (it != m_Uniforms.end())
		{
			return it->Location;
		}

		int32_t location = glGetUniformLocation(m_ProgramId, name.c_str());
		ASSERT_CORE(location > -1, "Invalid uniform location");

		return location;
	}

	/*std::unordered_map<std::string, Shader::ShaderUniformType> OpenGLShader::GetUniforms()
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

			if (!strcmp(name, "ViewProjectionMatrix"))
			{
				map[std::string(name, nameLength)] = GetUniformType(varType);
			}
		}

		return map;
	}*/

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
#pragma endregion
}