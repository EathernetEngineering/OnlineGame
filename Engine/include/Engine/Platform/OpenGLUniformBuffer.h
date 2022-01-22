#pragma once
#include <Engine/UniformBuffer.h>

namespace cee
{
	namespace engine
	{
		class OpenGLUniformBuffer : public UniformBuffer
		{
		public:
			OpenGLUniformBuffer(uint32_t size, uint32_t binding);
			virtual ~OpenGLUniformBuffer();
			
			virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
			
		private:
			uint32_t m_RendererId;
		};
	}
}
