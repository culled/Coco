#pragma once
#include "Core/Base.h"

#include "Shader.h"
#include <any>

namespace Coco
{
	struct COCO_API MaterialLayoutElement
	{
		std::string Name = "";
		ShaderDataType Type = ShaderDataType::None;
		uint32_t Offset = 0;
		uint32_t Size = 0;
		std::any Value;

		MaterialLayoutElement() = default;
		MaterialLayoutElement(std::string name, ShaderDataType type) :
			Name(name), Type(type) {}

		bool operator== (const MaterialLayoutElement& other);
	};

	class COCO_API MaterialLayout
	{
	public:
		MaterialLayout() = default;
		MaterialLayout(std::initializer_list<MaterialLayoutElement> elements)
		{
			Initialize(elements);
		}

		uint32_t GetSize() { return m_Size; }
		MaterialLayoutElement& GetElement(std::string name);

	private:
		void Initialize(const std::vector<MaterialLayoutElement>& elements);

		std::unordered_map<std::string, MaterialLayoutElement> m_Elements;
		uint32_t m_Size = 0;
	};

	class COCO_API Material
	{
	public:
		Material() = default;
		Material(const Ref<Shader>& shader);
		virtual ~Material();

		void Use();

		Ref<Shader> GetShader() { return m_Shader; }

		void SetLayout(uint32_t location, MaterialLayout layout);
		void SetInt(const std::string& name, const int& value);

		void SetFloat(const std::string& name, const float& value);
		void SetVector2(const std::string& name, const glm::vec2& value);
		void SetVector3(const std::string& name, const glm::vec3& value);
		void SetVector4(const std::string& name, const glm::vec4& value);

		void SetMatrix3(const std::string& name, const glm::mat3& value);
		void SetMatrix4(const std::string& name, const glm::mat4& value);

		bool operator ==(const Material& other);

	private:
		Ref<Shader> m_Shader = nullptr;
		Ref<UniformBuffer> m_Buffer = nullptr;
		MaterialLayout m_Layout;
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

