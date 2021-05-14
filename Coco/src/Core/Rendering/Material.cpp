#include "ccpch.h"
#include "Material.h"

#include "Core/App/Log.h"

#include <glm/gtc/type_ptr.hpp>

namespace Coco
{
	Material::Material(const std::string& name, const Ref<Shader>& shader) :
		m_Name(name), m_Shader(shader)
	{
		const auto& uniforms = shader->GetUniforms();

		for (const auto& uniform : uniforms)
		{
			m_Properties.emplace_back(uniform);
		}
	}

	Material::~Material()
	{
	}

	void Material::Bind()
	{
		m_Shader->Bind();

		for (const auto& prop : m_Properties)
		{
			if (prop.Value.has_value())
			{
				switch (prop.Type)
				{
				case ShaderDataType::Int: m_Shader->SetInt(prop.Name, std::any_cast<int>(prop.Value)); break;

				case ShaderDataType::Float: m_Shader->SetFloat(prop.Name, std::any_cast<float>(prop.Value)); break;
				case ShaderDataType::Float2: m_Shader->SetVector2(prop.Name, std::any_cast<glm::vec2>(prop.Value)); break;
				case ShaderDataType::Float3: m_Shader->SetVector3(prop.Name, std::any_cast<glm::vec3>(prop.Value)); break;
				case ShaderDataType::Float4: m_Shader->SetVector4(prop.Name, std::any_cast<glm::vec4>(prop.Value)); break;

				case ShaderDataType::Mat3: m_Shader->SetMatrix3(prop.Name, std::any_cast<glm::mat3>(prop.Value)); break;
				case ShaderDataType::Mat4: m_Shader->SetMatrix4(prop.Name, std::any_cast<glm::mat4>(prop.Value)); break;

				case ShaderDataType::Sampler2D:
					auto& texture = std::any_cast<Ref<Texture2D>>(prop.Value);

					if (texture)
						texture->Bind(prop.Location);
					break;
				}
			}
		}
	}

	void Material::SetInt(const std::string& name, const int& value)
	{
		auto& prop = GetProperty(name);
		prop.Value = value;
	}

	void Material::SetFloat(const std::string& name, const float& value)
	{
		auto& prop = GetProperty(name);
		prop.Value = value;
	}

	void Material::SetVector2(const std::string& name, const glm::vec2& value)
	{
		auto& prop = GetProperty(name);
		prop.Value = value;
	}

	void Material::SetVector3(const std::string& name, const glm::vec3& value)
	{
		auto& prop = GetProperty(name);
		prop.Value = value;
	}

	void Material::SetVector4(const std::string& name, const glm::vec4& value)
	{
		auto& prop = GetProperty(name);
		prop.Value = value;
	}

	void Material::SetMatrix3(const std::string& name, const glm::mat3& value)
	{
		auto& prop = GetProperty(name);
		prop.Value = value;
	}

	void Material::SetMatrix4(const std::string& name, const glm::mat4& value)
	{
		auto& prop = GetProperty(name);
		prop.Value = value;
	}

	void Material::SetTexture2D(const std::string& name, const Ref<Texture2D>& value)
	{
		auto& prop = GetProperty(name);
		prop.Value = value;
	}

	bool Material::operator==(const Material& other)
	{
		return false;
	}

	MaterialProperty& Material::GetProperty(const std::string& name)
	{
		std::vector<MaterialProperty>::iterator it = std::find_if(m_Properties.begin(), m_Properties.end(), [&](const auto& prop) {
			return prop.Name == name;
			});

		if (it == m_Properties.end())
		{
			ASSERT_CORE(false, "Could not find property");
			return *m_Properties.begin();
		}

		return *it;
	}

	std::unordered_map<std::string, Ref<Material>> MaterialLibrary::s_Materials;

	void MaterialLibrary::Add(const std::string& name, const Ref<Material>& material)
	{
		ASSERT_CORE(!Exists(name), "Material already added");
		s_Materials[name] = material;
	}

	Ref<Material> MaterialLibrary::Create(const std::string& name, const Ref<Shader>& shader)
	{
		auto material = CreateRef<Material>(name, shader);
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
