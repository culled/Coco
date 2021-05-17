#pragma once
#include "Core/Base.h"

#include "Buffers.h"

#include <glm/glm.hpp>

namespace Coco
{
	struct COCO_API ShaderUniform
	{
		std::string Name = "";
		ShaderDataType Type = ShaderDataType::None;
		uint32_t Location = 0;

		ShaderUniform() = default;
		ShaderUniform(const std::string& name, ShaderDataType type, uint32_t location) :
			Name(name), Type(type), Location(location) {}
	};

	class COCO_API Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetMatrix4(const std::string & name, const glm::mat4 & matrix) = 0;
		virtual void SetMatrix3(const std::string & name, const glm::mat3 & matrix) = 0;

		virtual void SetVector4(const std::string & name, const glm::vec4 & vector) = 0;
		virtual void SetVector3(const std::string & name, const glm::vec3 & vector) = 0;
		virtual void SetVector2(const std::string & name, const glm::vec2 & vector) = 0;

		virtual void SetFloat(const std::string & name, const float& value) = 0;
		virtual void SetInt(const std::string & name, const int& value) = 0;
		virtual void SetIntArray(const std::string & name, int* values, uint32_t count) = 0;

		virtual void BindBuffer(const Ref<UniformBuffer>& buf, uint32_t location) = 0;

		virtual const std::string& GetName() = 0;
		virtual const std::vector<ShaderUniform>& GetUniforms() const = 0;

		static Ref<Shader> CreateFromFile(const std::string& shaderPath, const std::string& name = "");
		static Shader* CreateAsset(const std::string& shaderPath);
		static Ref<Shader> CreateFromSource(const std::string & name, const std::string & vertSource, const std::string & fragSource);
	};

	class COCO_API ShaderLibrary
	{
	public:
		static void Add(const Ref<Shader>& shader);
		static Ref<Shader> Load(const std::string& filepath, const std::string& name = "");

		static Ref<Shader> Get(const std::string& name);
		static bool Exists(const std::string& name);
	private:
		static std::unordered_map<std::string, Ref<Shader>> s_Shaders;
	};
}

