#pragma once
#include "Core/Base.h"

#include "Core/App/Log.h"

namespace Coco
{
#pragma region Shader Data Types
	/*@brief Represents various datatypes used by shaders*/
	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool
	};

	/*@brief Gets the number of size in bytes of a given ShaderDataType
	* @param type - The type of shader data
	*
	* @returns The size of the data in bytes
	*/
	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 4 * 2;
		case ShaderDataType::Float3:	return 4 * 3;
		case ShaderDataType::Float4:	return 4 * 4;
		case ShaderDataType::Mat3:		return 4 * 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4 * 4;
		case ShaderDataType::Int:			return 4;
		case ShaderDataType::Int2:		return 4 * 2;
		case ShaderDataType::Int3:		return 4 * 3;
		case ShaderDataType::Int4:		return 4 * 4;
		case ShaderDataType::Bool:		return 1;
		}

		ASSERT_CORE(false, "Unknown shader data type");
		return 0;
	}

	/*@brief Gets the number of individual components that represent a given ShaderDataType
	* @param type - The type of shader data
	*
	* @returns The number of components that make up the data type
	*/
	static uint32_t ShaderDataTypeComponentCount(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return 1;
		case ShaderDataType::Float2:	return 2;
		case ShaderDataType::Float3:	return 3;
		case ShaderDataType::Float4:	return 4;
		case ShaderDataType::Mat3:		return 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4;
		case ShaderDataType::Int:			return 1;
		case ShaderDataType::Int2:		return 2;
		case ShaderDataType::Int3:		return 3;
		case ShaderDataType::Int4:		return 4;
		case ShaderDataType::Bool:		return 1;
		}

		ASSERT_CORE(false, "Unknown shader data type");
		return 0;
	}
#pragma endregion

#pragma region VertexBuffer Layout
	/*@brief Represents an array of data in a VertexBuffer*/
	struct COCO_API BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement() :
			Type(ShaderDataType::None), Name(""), Size(0), Offset(0),
			m_ComponentCount(0), Normalized(false)
		{}

		/*@brief Creates a new element that defines a vertex attribute
		* 		@param type - The type of data this element represents
		* 		@param name - The name of this element
		* 		@param normalized - If the graphics library should normalize the data this element represents
		*/
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Type(type), Name(name), Size(ShaderDataTypeSize(type)), Offset(0),
			m_ComponentCount(ShaderDataTypeComponentCount(type)), Normalized(normalized)
		{}

		/*@brief Gets the number of components representing this element
		* 		@returns The number of components that represent this element
		*/
		inline uint32_t GetComponentCount() const { return m_ComponentCount; }
	private:
		uint32_t m_ComponentCount;
	};

	class COCO_API VertexBufferLayout
	{
	public:
		VertexBufferLayout()
			: m_Stride(0)
		{}

		/*@brief Creates a layout from an initializer list of BufferElements*/
		VertexBufferLayout(const std::initializer_list<BufferElement>& elements)
			:m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		/*@brief Gets the array of BufferElements that make up this layout
		* 		@returns The BufferElements that make up this layout
		*/
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		/*@brief Gets the stride between each vertex's data
		* 		@returns The number of bytes each vertex's data takes up
		*/
		inline uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		/*@brief Calculates the offsets for each BufferElement in this layout. Also calculates the total stride between verticies*/
		void CalculateOffsetsAndStride();

		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride;
	};
#pragma endregion

	class COCO_API VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		/*@brief Binds this vertex buffer for use*/
		virtual void Bind() const = 0;

		/*@brief Unbinds this vertex buffer from use*/
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual void SetLayout(const VertexBufferLayout& layout) = 0;
		virtual const VertexBufferLayout& GetLayout() = 0;

		/*@brief Creates a vertex buffer for the given array of verticies
		* 		@param verticies - An array of vertex attribute values
		* 		@param size - The size (in bytes) of the verticies array
		*/
		static Ref<VertexBuffer> Create(float* verticies, uint32_t size);
		static Ref<VertexBuffer> Create(uint32_t size);
	};

	class COCO_API IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		/*@brief Binds this index buffer for use*/
		virtual void Bind() const = 0;

		/*@brief Unbinds this index buffer from use*/
		virtual void Unbind() const = 0;

		/*@brief Gets the number of indicies in this buffer
		* 		@returns The number of indicies in this buffer
		*/
		virtual uint32_t GetCount() const = 0;

		/*@brief Creates an index layout for the given array of indicies
		* 		@param indicies - An array of indicies
		* 		@param count - The number of indicies given
		*/
		static Ref<IndexBuffer> Create(uint32_t * indicies, uint32_t count);
	};
}

