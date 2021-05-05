#pragma once
#include "Core/Base.h"

#include "Core/Rendering/Shader.h"
#include "glad/glad.h"

namespace Coco {
	class COCO_API OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filePath, const std::string& name = "");
		OpenGLShader(const std::string& name, const std::string& vertSource, const std::string& fragSource);
		virtual ~OpenGLShader();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual const std::string& GetName() override { return m_Name; }
		virtual std::unordered_map<std::string, Shader::ShaderUniformType> GetUniforms() override;

		virtual void SetMatrix4(const std::string& name, const glm::mat4& matrix) override;
		virtual void SetMatrix3(const std::string& name, const glm::mat3& matrix) override;

		virtual void SetVector4(const std::string& name, const glm::vec4& vector) override;
		virtual void SetVector3(const std::string& name, const glm::vec3& vector) override;
		virtual void SetVector2(const std::string& name, const glm::vec2& vector) override;

		virtual void SetFloat(const std::string& name, const float& value) override;
		virtual void SetInt(const std::string& name, const int& value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;

	private:
		int32_t GetUniformLocation(const std::string& name);
		uint32_t m_ProgramId;
		std::string m_Name;
		std::unordered_map<std::string, uint32_t> m_UniformCache;

	private:
		std::string ReadFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

	};
}