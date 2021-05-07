#include "ccpch.h"
#include "Material.h"

#include "Core/App/Log.h"

namespace Coco
{
	static Scope<MaterialProperty> GetProperty(Shader::ShaderUniformType uniformType)
	{
		switch (uniformType)
		{
		case Shader::ShaderUniformType::Int:
			return CreateScope<IntMaterialProperty>();
		case Shader::ShaderUniformType::Float:
			return CreateScope<FloatMaterialProperty>();
		case Shader::ShaderUniformType::Float2:
			return CreateScope<Vector2MaterialProperty>();
		case Shader::ShaderUniformType::Float3:
			return CreateScope<Vector3MaterialProperty>();
		case Shader::ShaderUniformType::Float4:
			return CreateScope<Vector4MaterialProperty>();
		case Shader::ShaderUniformType::Mat3:
			return CreateScope<Mat3MaterialProperty>();
		case Shader::ShaderUniformType::Mat4:
			return CreateScope<Mat4MaterialProperty>();
		}

		ASSERT_CORE(false, "Unknown shader uniform type");
		return nullptr;
	}

	Material::Material(const Ref<Shader>& shader) :
		m_Shader(shader)
	{
		for (auto it : shader->GetUniforms())
		{
			m_Uniforms[it.first] = GetProperty(it.second);
		}
	}

	Material::~Material()
	{
	}

	void Material::Use()
	{
		m_Shader->Bind();

		for (auto& it : m_Uniforms)
		{
			switch (it.second->Type)
			{
			case Shader::ShaderUniformType::Float:
				m_Shader->SetFloat(it.first, static_cast<FloatMaterialProperty*>(it.second.get())->Value);
				break;
			case Shader::ShaderUniformType::Float4:
				m_Shader->SetVector4(it.first, static_cast<Vector4MaterialProperty*>(it.second.get())->Value);
				break;
			case Shader::ShaderUniformType::Mat4:
				m_Shader->SetMatrix4(it.first, static_cast<Mat4MaterialProperty*>(it.second.get())->Value);
				break;
			}
		}
	}

	void Material::SetInt(const std::string& name, const int& value)
	{
		MaterialProperty* baseProp = m_Uniforms[name].get();

		ASSERT_CORE(baseProp->Type == Shader::ShaderUniformType::Int, "Wrong data type");

		IntMaterialProperty* prop = static_cast<IntMaterialProperty*>(m_Uniforms[name].get());
		prop->Value = value;
	}

	void Material::SetFloat(const std::string& name, const float& value)
	{
		MaterialProperty* baseProp = m_Uniforms[name].get();

		ASSERT_CORE(baseProp->Type == Shader::ShaderUniformType::Float, "Wrong data type");

		FloatMaterialProperty* prop = static_cast<FloatMaterialProperty*>(m_Uniforms[name].get());
		prop->Value = value;
	}

	void Material::SetVector2(const std::string& name, const glm::vec2& value)
	{
		MaterialProperty* baseProp = m_Uniforms[name].get();

		ASSERT_CORE(baseProp->Type == Shader::ShaderUniformType::Float2, "Wrong data type");

		Vector2MaterialProperty* prop = static_cast<Vector2MaterialProperty*>(m_Uniforms[name].get());
		prop->Value = value;
	}

	void Material::SetVector3(const std::string& name, const glm::vec3& value)
	{
		MaterialProperty* baseProp = m_Uniforms[name].get();

		ASSERT_CORE(baseProp->Type == Shader::ShaderUniformType::Float3, "Wrong data type");

		Vector3MaterialProperty* prop = static_cast<Vector3MaterialProperty*>(m_Uniforms[name].get());
		prop->Value = value;
	}

	void Material::SetVector4(const std::string& name, const glm::vec4& value)
	{
		MaterialProperty* baseProp = m_Uniforms[name].get();

		ASSERT_CORE(baseProp->Type == Shader::ShaderUniformType::Float4, "Wrong data type");

		Vector4MaterialProperty* prop = static_cast<Vector4MaterialProperty*>(m_Uniforms[name].get());
		prop->Value = value;
	}

	void Material::SetMatrix3(const std::string& name, const glm::mat3& value)
	{
		MaterialProperty* baseProp = m_Uniforms[name].get();

		ASSERT_CORE(baseProp->Type == Shader::ShaderUniformType::Mat3, "Wrong data type");

		Mat3MaterialProperty* prop = static_cast<Mat3MaterialProperty*>(m_Uniforms[name].get());
		prop->Value = value;
	}

	void Material::SetMatrix4(const std::string& name, const glm::mat4& value)
	{
		MaterialProperty* baseProp = m_Uniforms[name].get();

		ASSERT_CORE(baseProp->Type == Shader::ShaderUniformType::Mat4, "Wrong data type");

		Mat4MaterialProperty* prop = static_cast<Mat4MaterialProperty*>(m_Uniforms[name].get());
		prop->Value = value;
	}

	std::unordered_map<std::string, Ref<Material>> MaterialLibrary::s_Materials;

	void MaterialLibrary::Add(const std::string& name, const Ref<Material>& material)
	{
		ASSERT_CORE(!Exists(name), "Material already added");
		s_Materials[name] = material;
	}

	Ref<Material> MaterialLibrary::Create(const std::string& name, const Ref<Shader>& shader)
	{
		auto material = CreateRef<Material>(shader);
		Add(name, material);
		return material;
	}

	Ref<Material> MaterialLibrary::Get(const std::string& name)
	{
		ASSERT_CORE(Exists(name), "Material not found");
		return s_Materials[name];
	}

	bool MaterialLibrary::Exists(const std::string& name)
	{
		return s_Materials.find(name) != s_Materials.end();
	}
}
