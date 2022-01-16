#pragma once

#include <glm/glm.hpp>

namespace cee
{
	namespace engine
	{
		class Shader
		{
		public:
			virtual ~Shader() = default;
			
			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;
			
			virtual void SetInt(const std::string& name, int value) = 0;
			virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
			virtual void SetFloat(const std::string& name, float value) = 0;
			virtual void SetFloat2(const std::string& name, glm::vec2 values) = 0;
			virtual void SetFloat3(const std::string& name, glm::vec3 values) = 0;
			virtual void SetFloat4(const std::string& name, glm::vec4 values) = 0;
			virtual void SetMat4(const std::string& name, glm::mat4 matrix) = 0;
			
			virtual const std::string& GetName() const = 0;
			
			static std::shared_ptr<Shader> Create(const std::string& vertexFilename, const std::string& FragmentFilename);
		};
	}
}
