#pragma once
#include <Engine/Shader.h>

#include <unordered_map>
#include <glad/glad.h>

namespace cee
{
	namespace engine
	{
		class OpenGLShader : public Shader
		{
		public:
			OpenGLShader(const std::string& vertexFilename, const std::string& fragmentFilename);
			virtual ~OpenGLShader();
			
			virtual void Bind() const override;
			virtual void Unbind() const override;
			
			virtual void SetInt(const std::string& name, int value) override;
			virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
			virtual void SetFloat(const std::string& name, float value) override;
			virtual void SetFloat2(const std::string& name, glm::vec2 values) override;
			virtual void SetFloat3(const std::string& name, glm::vec3 values) override;
			virtual void SetFloat4(const std::string& name, glm::vec4 values) override;
			virtual void SetMat4(const std::string& name, glm::mat4 matrix) override;
			
			virtual const std::string& GetName() const override { return m_Name; }
			
		private:
			void ReadFile(const std::string& filepath, GLenum shaderType);
			void CreateProgram();
			
		private:
			std::unordered_map<std::string, GLint> m_UniformCache;
			
		private:
			std::string m_Name;
			uint32_t m_RendererId;
			
		private:
			std::unordered_map<GLenum, std::vector<uint8_t>> m_SourceCode;
		};
	}
}
