#include "ccpch.h"
#include "Material.h"

#include "Core/App/Log.h"

#include <glm/gtc/type_ptr.hpp>

namespace Coco
{
	const MaterialLayoutElement& MaterialLayout::GetElement(std::string name)
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

		const auto& element = m_Layout.GetElement(name);
		m_Buffer->SetData(&value, element.Size, element.Offset);
	}

	void Material::SetFloat(const std::string& name, const float& value)
	{
		ASSERT_CORE(m_Buffer, "Buffer has not been created");

		const auto& element = m_Layout.GetElement(name);
		m_Buffer->SetData(&value, element.Size, element.Offset);
	}

	void Material::SetVector2(const std::string& name, const glm::vec2& value)
	{
		ASSERT_CORE(m_Buffer, "Buffer has not been created");

		const auto& element = m_Layout.GetElement(name);
		m_Buffer->SetData(glm::value_ptr(value), element.Size, element.Offset);
	}

	void Material::SetVector3(const std::string& name, const glm::vec3& value)
	{
		ASSERT_CORE(m_Buffer, "Buffer has not been created");

		const auto& element = m_Layout.GetElement(name);
		m_Buffer->SetData(glm::value_ptr(value), element.Size, element.Offset);
	}

	void Material::SetVector4(const std::string& name, const glm::vec4& value)
	{
		ASSERT_CORE(m_Buffer, "Buffer has not been created");

		const auto& element = m_Layout.GetElement(name);
		m_Buffer->SetData(glm::value_ptr(value), element.Size, element.Offset);
	}

	void Material::SetMatrix3(const std::string& name, const glm::mat3& value)
	{
		ASSERT_CORE(m_Buffer, "Buffer has not been created");

		const auto& element = m_Layout.GetElement(name);
		m_Buffer->SetData(glm::value_ptr(value), element.Size, element.Offset);
	}

	void Material::SetMatrix4(const std::string& name, const glm::mat4& value)
	{
		ASSERT_CORE(m_Buffer, "Buffer has not been created");

		const auto& element = m_Layout.GetElement(name);
		m_Buffer->SetData(glm::value_ptr(value), element.Size, element.Offset);
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
