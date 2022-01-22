#pragma once
#include <Engine/Texture.h>

#include <glad/glad.h>

namespace cee
{
	namespace engine
	{
		class OpenGLTexture2D : public Texture2D
		{
		public:
			OpenGLTexture2D(uint32_t width, uint32_t height);
			OpenGLTexture2D(const std::string& filepath);
			virtual ~OpenGLTexture2D();
			
			virtual uint32_t GetWidth() const override { return m_Width; }
			virtual uint32_t GetHeight() const override { return m_Height; }
			virtual uint32_t GetRendererId() const override { return m_RendererId; }
			
			virtual void SetData(void* data, uint32_t size) override;
			
			virtual void Bind(uint32_t slot = 0) const override;
			
			virtual bool IsLoaded() const override { return m_IsLoaded; }
			
			virtual bool operator==(const Texture& other) const override
			{
				return m_RendererId == ((OpenGLTexture2D&)other).m_RendererId;
			}
			
		private:
			std::string m_Filepath;
			bool m_IsLoaded;
			uint32_t m_Width, m_Height;
			uint32_t m_RendererId;
			GLenum m_InternalFormat, m_DataFormat;
		};
	}
}
