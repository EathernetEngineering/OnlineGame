#include <Engine/pch.h>
#include <Engine/Platform/OpenGLShader.h>

#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

namespace cee
{
	namespace engine
	{
		OpenGLShader::OpenGLShader(const std::string& vertexFilename, const std::string& fragmentFilename)
		{
			ReadFile(vertexFilename, GL_VERTEX_SHADER);
			ReadFile(fragmentFilename, GL_FRAGMENT_SHADER);
			CreateProgram();
		}
		
		OpenGLShader::~OpenGLShader()
		{
			glDeleteProgram(m_RendererId);
		}
		
		void OpenGLShader::Bind() const
		{
			glUseProgram(m_RendererId);
		}
		
		void OpenGLShader::Unbind() const
		{
			glUseProgram(0);
		}
		
		void OpenGLShader::SetInt(const std::string& name, int value)
		{
			
			if (m_UniformCache.find(name) == m_UniformCache.end())
			{
				m_UniformCache[name] = glGetUniformLocation(m_RendererId, name.c_str());
			}
			glUniform1i(m_UniformCache[name], value);
		}
		
		void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
		{
			if (m_UniformCache.find(name) == m_UniformCache.end())
			{
				m_UniformCache[name] = glGetUniformLocation(m_RendererId, name.c_str());
			}
			glUniform1iv(m_UniformCache[name], count, values);
		}
		
		void OpenGLShader::SetFloat(const std::string& name, float value)
		{
			if (m_UniformCache.find(name) == m_UniformCache.end())
			{
				m_UniformCache[name] = glGetUniformLocation(m_RendererId, name.c_str());
			}
			glUniform1f(m_UniformCache[name], value);
		}
		
		void OpenGLShader::SetFloat2(const std::string& name, glm::vec2 values)
		{
			if (m_UniformCache.find(name) == m_UniformCache.end())
			{
				m_UniformCache[name] = glGetUniformLocation(m_RendererId, name.c_str());
			}
			glUniform2f(m_UniformCache[name], values.x, values.y);
		}
		
		void OpenGLShader::SetFloat3(const std::string& name, glm::vec3 values)
		{
			if (m_UniformCache.find(name) == m_UniformCache.end())
			{
				m_UniformCache[name] = glGetUniformLocation(m_RendererId, name.c_str());
			}
			glUniform3f(m_UniformCache[name], values.x, values.y, values.z);
		}
		
		void OpenGLShader::SetFloat4(const std::string& name, glm::vec4 values)
		{
			if (m_UniformCache.find(name) == m_UniformCache.end())
			{
				m_UniformCache[name] = glGetUniformLocation(m_RendererId, name.c_str());
			}
			glUniform4f(m_UniformCache[name], values.x, values.y, values.z, values.w);
		}
		
		void OpenGLShader::SetMat4(const std::string& name, glm::mat4 matrix)
		{
			if (m_UniformCache.find(name) == m_UniformCache.end())
			{
				m_UniformCache[name] = glGetUniformLocation(m_RendererId, name.c_str());
			}
			glUniformMatrix4fv(m_UniformCache[name], 1, GL_FALSE, glm::value_ptr(matrix));
		}
		
		void OpenGLShader::ReadFile(const std::string& filepath, GLenum shaderType)
		{
			std::ifstream file(filepath);
			
			CEE_CORE_ASSERT(file.is_open());
			
			file.seekg(0, file.end);
			uint32_t length = file.tellg();
			file.seekg(0, file.beg);
			
			if(!m_SourceCode[shaderType].empty())
			{
				CEE_CORE_WARN("Shader {0} already parsed. clearing and re-parsing", m_Name.c_str());
				
				m_SourceCode[shaderType].clear();
				m_SourceCode[shaderType].shrink_to_fit();
			}
			
			char* data = new char[length + 1];
			data[length] = '\0';
			
			m_SourceCode[shaderType].reserve(length + 1);
			file.read(data, length);
			
			m_SourceCode[shaderType].assign(data, data + length + 1);
			
			delete[] data;
			
			file.close();
		}
		
		void OpenGLShader::CreateProgram()
		{
			uint32_t vertexId, fragmentId;
			int success;
			char errorMessage[512];
			
			// Vertex shader.
			{
				uint8_t* vertexSource = m_SourceCode[GL_VERTEX_SHADER].data();
				
				vertexId = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(vertexId, 1, (const char**)&vertexSource, NULL);
				glCompileShader(vertexId);
				glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(vertexId, 512, NULL, errorMessage);
					CEE_CORE_ERROR("Create shader (vertex) failed: {0}", errorMessage);
					return;
				}
			}
			// Fragment shader.
			{
				uint8_t* fragmentSource = m_SourceCode[GL_FRAGMENT_SHADER].data();
				
				fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(fragmentId, 1, (const char**)&fragmentSource, NULL);
				glCompileShader(fragmentId);
				glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(fragmentId, 512, NULL, errorMessage);
					CEE_CORE_ERROR("Create shader (fragment) failed: {0}", errorMessage);
					return;
				}
			}
			
			m_RendererId = glCreateProgram();
			glAttachShader(m_RendererId, vertexId);
			glAttachShader(m_RendererId, fragmentId);
			glLinkProgram(m_RendererId);
			
			glGetProgramiv(m_RendererId, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(m_RendererId, 512, NULL, errorMessage);
				CEE_CORE_ERROR("Link shader failed: {0}", errorMessage);
			}
			glDeleteShader(vertexId);
			glDeleteShader(fragmentId);
		}
	}
}
