#pragma once
#include "Core/Base.h"

#include "Core/Rendering/Shader.h"
#include "glad/glad.h"

namespace Coco {
	class COCO_API OpenGLShader : public Shader
	{
	public:
		using ShaderMap = std::unordered_map<GLenum, std::string>;

		OpenGLShader(const std::string& filePath, const std::string& name = "");
		OpenGLShader(const std::string& name, const std::string& vertSource, const std::string& fragSource);
		virtual ~OpenGLShader();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual const std::string& GetName() override { return m_Name; }
		virtual const std::vector<ShaderUniform>& GetUniforms() const override { return m_Uniforms; }

		virtual void SetMatrix4(const std::string& name, const glm::mat4& matrix) override;
		virtual void SetMatrix3(const std::string& name, const glm::mat3& matrix) override;

		virtual void SetVector4(const std::string& name, const glm::vec4& vector) override;
		virtual void SetVector3(const std::string& name, const glm::vec3& vector) override;
		virtual void SetVector2(const std::string& name, const glm::vec2& vector) override;

		virtual void SetFloat(const std::string& name, const float& value) override;
		virtual void SetInt(const std::string& name, const int& value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;

		virtual void BindBuffer(const Ref<UniformBuffer>& buf, uint32_t location) override;

		static std::string s_CacheDirectory;

	private:
		uint32_t m_ProgramId;
		std::string m_Name;
		std::string m_FilePath;
		std::unordered_map < GLenum, std:: vector<uint32_t >> m_VulkanSpirV; //Holds bytecode for each Vulkan vertex/fragment shader
		std::unordered_map < GLenum, std:: vector<uint32_t >> m_OpenGLSpirV; //Holds bytecode for each OpenGL vertex/fragment shader
		ShaderMap m_OpenGLSourceCode;
		std::vector<ShaderUniform> m_Uniforms;

	private:
		int32_t GetUniformLocation(const std::string& name);
		std::string ReadFile(const std::string& filePath);
		ShaderMap PreProcess(const std::string& source);
		void CompileOrGetVulkanBinaries(const ShaderMap& shaderSources);
		void CompileOrGetOpenGLBinaries();
		void CreateProgram();
		void Reflect(GLenum type, const std::vector<uint32_t>& shaderData);
		void GetUniforms(GLenum type, const std::vector<uint32_t>& shaderData);
	};
}