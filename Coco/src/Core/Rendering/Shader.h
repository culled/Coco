#pragma once
#include "Core/Base.h"

#include "glm/glm.hpp"

namespace Coco
{
	class COCO_API Shader
	{
	public:
		enum class ShaderUniformType
		{
			None = 0,
			Float,
			Int,
			Bool,
			Float2,
			Float3,
			Float4,
			Mat3,
			Mat4,
			Sampler2D
		};

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

		virtual const std::string& GetName() = 0;
		virtual std::unordered_map<std::string, ShaderUniformType> GetUniforms() = 0;

		static Ref<Shader> CreateFromFile(const std::string & shaderPath, const std::string & name = "");
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

