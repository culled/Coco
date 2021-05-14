#pragma once
#include "Core/Base.h"

#include "Shader.h"
#include "Textures.h"
#include <any>

namespace Coco
{
	struct COCO_API MaterialProperty : public ShaderUniform
	{
		std::any Value;

		MaterialProperty(const ShaderUniform& uniform) : ShaderUniform(uniform)
		{
			switch (uniform.Type)
			{
			case ShaderDataType::Int: Value = 0; break;
			case ShaderDataType::Int2: Value = glm::ivec2(0); break;
			case ShaderDataType::Int3: Value = glm::ivec3(0); break;
			case ShaderDataType::Int4: Value = glm::ivec4(0); break;

			case ShaderDataType::Float: Value = 0.0f; break;
			case ShaderDataType::Float2: Value = glm::vec2(0.0f); break;
			case ShaderDataType::Float3: Value = glm::vec3(0.0f); break;
			case ShaderDataType::Float4: Value = glm::vec4(0.0f); break;

			case ShaderDataType::Mat3: Value = glm::mat3(1.0f); break;
			case ShaderDataType::Mat4: Value = glm::mat4(1.0f); break;

			case ShaderDataType::Bool: Value = false; break;
			}
		}
	};

	class COCO_API Material
	{
	public:
		Material() = default;
		Material(const std::string& name, const Ref<Shader>& shader);
		virtual ~Material();

		void Bind();

		const Ref<Shader>& GetShader() const { return m_Shader; }
		const std::string& GetName() const { return m_Name; }
		std::vector<MaterialProperty>& GetProperties() { return m_Properties; }

		void SetInt(const std::string& name, const int& value);

		void SetFloat(const std::string& name, const float& value);
		void SetVector2(const std::string& name, const glm::vec2& value);
		void SetVector3(const std::string& name, const glm::vec3& value);
		void SetVector4(const std::string& name, const glm::vec4& value);

		void SetMatrix3(const std::string& name, const glm::mat3& value);
		void SetMatrix4(const std::string& name, const glm::mat4& value);

		void SetTexture2D(const std::string& name, const Ref<Texture2D>& value);

		bool operator ==(const Material& other);

	private:
		Ref<Shader> m_Shader = nullptr;
		std::string m_Name;
		std::vector<MaterialProperty> m_Properties;

	private:
		MaterialProperty& GetProperty(const std::string& name);
	};

	class COCO_API MaterialLibrary
	{
	public:
		static void Add(const std::string& name, const Ref<Material>& material);
		static Ref<Material> Create(const std::string& name, const Ref<Shader>& shader);

		static Ref<Material> Get(const std::string& name);
		static bool Exists(const std::string& name);
	private:
		static std::unordered_map<std::string, Ref<Material>> s_Materials;
	};
}

