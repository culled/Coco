#pragma once
#include "Core/Base.h"

#include "Shader.h"

namespace Coco
{
#pragma region Material Properties
	struct COCO_API MaterialProperty
	{
		Shader::ShaderUniformType Type = Shader::ShaderUniformType::None;

		MaterialProperty() = default;
		MaterialProperty(const Shader::ShaderUniformType& type) : Type(type) {}
		virtual ~MaterialProperty() = default;
	};

	struct COCO_API IntMaterialProperty : public MaterialProperty
	{
		int Value = 0;

		IntMaterialProperty() : MaterialProperty(Shader::ShaderUniformType::Int) {}
		virtual ~IntMaterialProperty() = default;

	};
	struct COCO_API FloatMaterialProperty : public MaterialProperty
	{
		float Value = 0.0f;

		FloatMaterialProperty() : MaterialProperty(Shader::ShaderUniformType::Float) {}
		virtual ~FloatMaterialProperty() = default;
	};

	struct COCO_API Vector2MaterialProperty : public MaterialProperty
	{
		glm::vec2 Value = glm::vec2(0.0f);

		Vector2MaterialProperty() : MaterialProperty(Shader::ShaderUniformType::Float2) {}
		virtual ~Vector2MaterialProperty() = default;
	};

	struct COCO_API Vector3MaterialProperty : public MaterialProperty
	{
		glm::vec3 Value = glm::vec3(0.0f);

		Vector3MaterialProperty() : MaterialProperty(Shader::ShaderUniformType::Float3) {}
		virtual ~Vector3MaterialProperty() = default;
	};

	struct COCO_API Vector4MaterialProperty : public MaterialProperty
	{
		glm::vec4 Value = glm::vec4(0.0f);

		Vector4MaterialProperty() : MaterialProperty(Shader::ShaderUniformType::Float4) {}
		virtual ~Vector4MaterialProperty() = default;
	};

	struct COCO_API Mat3MaterialProperty : public MaterialProperty
	{
		glm::mat3 Value = glm::mat3(0.0f);

		Mat3MaterialProperty() : MaterialProperty(Shader::ShaderUniformType::Mat3) {}
		virtual ~Mat3MaterialProperty() = default;
	};

	struct COCO_API Mat4MaterialProperty : public MaterialProperty
	{
		glm::mat4 Value = glm::mat4(0.0f);

		Mat4MaterialProperty() : MaterialProperty(Shader::ShaderUniformType::Mat4) {}
		virtual ~Mat4MaterialProperty() = default;
	};
#pragma endregion

	class COCO_API Material
	{
	public:
		Material() = default;
		Material(Ref<Shader>& shader);
		virtual ~Material();

		void Use();

		Ref<Shader> GetShader() { return m_Shader; }

		void SetInt(const std::string& name, const int& value);

		void SetFloat(const std::string& name, const float& value);
		void SetVector2(const std::string& name, const glm::vec2& value);
		void SetVector3(const std::string& name, const glm::vec3& value);
		void SetVector4(const std::string& name, const glm::vec4& value);

		void SetMatrix3(const std::string& name, const glm::mat3& value);
		void SetMatrix4(const std::string& name, const glm::mat4& value);

	private:
		Ref<Shader> m_Shader = nullptr;
		std::unordered_map<std::string, Scope<MaterialProperty>> m_Uniforms;
	};
}

