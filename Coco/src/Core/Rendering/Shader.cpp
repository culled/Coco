#include "ccpch.h"
#include "Shader.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Coco
{
	Ref<Shader> Shader::CreateFromFile(const std::string& shaderPath, const std::string& name)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT_CORE(false, "No render api");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(shaderPath, name);
		}

		ASSERT_CORE(false, "Invalid API");
		return nullptr;
	}

	Shader* Shader::CreateAsset(const std::string& shaderPath)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT_CORE(false, "No render api");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(shaderPath);
		}

		ASSERT_CORE(false, "Invalid API");
		return nullptr;
	}

	Ref<Shader> Shader::CreateFromSource(const std::string& name, const std::string& vertSource, const std::string& fragSource)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT_CORE(false, "No render api");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(name, vertSource, fragSource);
		}

		ASSERT_CORE(false, "Invalid API");
		return nullptr;
	}

	std::unordered_map<std::string, Ref<Shader>> ShaderLibrary::s_Shaders;

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& shaderName = shader->GetName();
		ASSERT_CORE(!Exists(shaderName), "Shader already added");
		s_Shaders[shaderName] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath, const std::string& name)
	{
		auto shader = Shader::CreateFromFile(filepath, name);
		Add(shader);
		return shader;
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return s_Shaders.find(name) != s_Shaders.end();
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		ASSERT_CORE(Exists(name), "Shader not found");
		return s_Shaders[name];
	}
}