#include "ccpch.h"
#include "Material.h"

#include "Core/App/Log.h"

#include <glm/gtc/type_ptr.hpp>

namespace Coco
{
	bool MaterialLayoutElement::operator==(const MaterialLayoutElement& other)
	{
		if (other.Type != this->Type) return false;

		if (!other.Value.has_value() || !this->Value.has_value()) return false;

		switch (this->Type)
		{
		case ShaderDataType::Bool:
			return std::any_cast<bool>(other.Value) == std::any_cast<bool>(this->Value);
		case ShaderDataType::Int:
			return std::any_cast<int>(other.Value) == std::any_cast<int>(this->Value);
		case ShaderDataType::Float:
			return std::any_cast<float>(other.Value) == std::any_cast<float>(this->Value);
		case ShaderDataType::Float2:
			return std::any_cast<glm::vec2>(other.Value) == std::any_cast<glm::vec2>(this->Value);
		case ShaderDataType::Float3:
			return std::any_cast<glm::vec3>(other.Value) == std::any_cast<glm::vec3>(this->Value);
		case ShaderDataType::Float4:
			return std::any_cast<glm::vec4>(other.Value) == std::any_cast<glm::vec4>(this->Value);
		case ShaderDataType::Int2:
			return std::any_cast<glm::ivec2>(other.Value) == std::any_cast<glm::ivec2>(this->Value);
		case ShaderDataType::Int3:
			return std::any_cast<glm::ivec3>(other.Value) == std::any_cast<glm::ivec3>(this->Value);
		case ShaderDataType::Int4:
			return std::any_cast<glm::ivec4>(other.Value) == std::any_cast<glm::ivec4>(this->Value);
		case ShaderDataType::Mat3:
			return std::any_cast<glm::mat3>(other.Value) == std::any_cast<glm::mat3>(this->Value);
		case ShaderDataType::Mat4:
			return std::any_cast<glm::mat4>(other.Value) == std::any_cast<glm::mat4>(this->Value);
		}

		ASSERT_CORE(false, "Invalid shader type");
	}

	MaterialLayoutElement& MaterialLayout::GetElement(std::string name)
	{
		ASSERT_CORE(m_Elements.find(name) != m_Elements.end(), "Could not find element with that name");
		return m_Elements[name];
	}

	void MaterialLayout::Initialize(const std::vector<MaterialLayoutElement>& elements)
	{
		m_Size = 0;

		for (auto& element : elements)
		{
			m_Elements[element.Name] = element;
			auto& e = m_Elements[element.Name];

			e.Size = ShaderDataTypeSize(element.Type);

			if (m_Size > 0)
			{
				e.Offset = m_Size + (uint32_t)(e.Size - (float)e.Size / (float)m_Size);
			}
			else
			{
				e.Offset = 0;
			}

			m_Size = e.Offset + e.Size;
		}
	}

	Material::Material(const Ref<Shader>& shader) :
		m_Shader(shader)
	{}

	Material::~Material()
	{
	}

	void Material::Use()
	{
		m_Shader->Bind();
	}

	void Material::SetLayout(uint32_t location, MaterialLayout layout)
	{
		m_Layout = layout;

		m_Buffer = UniformBuffer::Create(m_Layout.GetSize(), location);
	}

	void Material::SetInt(const std::string& name, const int& value)
	{
		ASSERT_CORE(m_Buffer, "Buffer has not been created");

		auto& element = m_Layout.GetElement(name);
		element.Value = value;
		m_Buffer->SetData(&value, element.Size, element.Offset);
	}

	void Material::SetFloat(const std::string& name, const float& value)
	{
		ASSERT_CORE(m_Buffer, "Buffer has not been created");

		auto& element = m_Layout.GetElement(name);
		element.Value = value;
		m_Buffer->SetData(&value, element.Size, element.Offset);
	}

	void Material::SetVector2(const std::string& name, const glm::vec2& value)
	{
		ASSERT_CORE(m_Buffer, "Buffer has not been created");

		auto& element = m_Layout.GetElement(name);
		element.Value = value;
		m_Buffer->SetData(glm::value_ptr(value), element.Size, element.Offset);
	}

	void Material::SetVector3(const std::string& name, const glm::vec3& value)
	{
		ASSERT_CORE(m_Buffer, "Buffer has not been created");

		auto& element = m_Layout.GetElement(name);
		element.Value = value;
		m_Buffer->SetData(glm::value_ptr(value), element.Size, element.Offset);
	}

	void Material::SetVector4(const std::string& name, const glm::vec4& value)
	{
		ASSERT_CORE(m_Buffer, "Buffer has not been created");

		auto& element = m_Layout.GetElement(name);
		element.Value = value;
		m_Buffer->SetData(glm::value_ptr(value), element.Size, element.Offset);
	}

	void Material::SetMatrix3(const std::string& name, const glm::mat3& value)
	{
		ASSERT_CORE(m_Buffer, "Buffer has not been created");

		auto& element = m_Layout.GetElement(name);
		element.Value = value;
		m_Buffer->SetData(glm::value_ptr(value), element.Size, element.Offset);
	}

	void Material::SetMatrix4(const std::string& name, const glm::mat4& value)
	{
		ASSERT_CORE(m_Buffer, "Buffer has not been created");

		auto& element = m_Layout.GetElement(name);
		element.Value = value;
		m_Buffer->SetData(glm::value_ptr(value), element.Size, element.Offset);
	}

	bool Material::operator==(const Material& other)
	{
		return false;
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
