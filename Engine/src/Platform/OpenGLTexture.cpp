#include <Engine/pch.h>
#include <Engine/Platform/OpenGLTexture.h>

#include <stb/stb_image.h>

namespace cee
{
	namespace engine
	{
		OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height)
		{
			CEE_PROFILE_FUNCTION();
			
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
			
			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
			glTextureStorage2D(m_RendererId, 1, m_InternalFormat, m_Width, m_Height);
			
			glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		
		OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath)
			: m_Filepath(filepath)
		{
			CEE_PROFILE_FUNCTION();
			
			int width, height, channels;
			stbi_set_flip_vertically_on_load(1);
			stbi_uc* data = nullptr;
			{
				CEE_PROFILE_SCOPE("stbi_load");
				data = stbi_load(m_Filepath.c_str(), &width, &height, &channels, 0);
			}
			
			if (data)
			{
				m_IsLoaded = true;
				
				m_Width = width;
				m_Height = height;
				
				GLenum internalFormat = 0, dataFormat = 0;
				if (channels == 4)
				{
					internalFormat = GL_RGBA8;
					dataFormat = GL_RGBA;
				}
				else if (channels == 3)
				{
					internalFormat = GL_RGB8;
					dataFormat = GL_RGB;
				}
				
				m_InternalFormat = internalFormat;
				m_DataFormat = dataFormat;
				
				CEE_CORE_ASSERT(internalFormat & dataFormat, "Format not supported");
				
				glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
				glTextureStorage2D(m_RendererId, 1, m_InternalFormat, m_Width, m_Height);
				
				glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				
				glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
				
				glTextureSubImage2D(m_RendererId, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
				
				stbi_image_free(data);
			}
		}
		
		OpenGLTexture2D::~OpenGLTexture2D()
		{
			CEE_PROFILE_FUNCTION();
			
			glDeleteTextures(1, &m_RendererId);
		}
		
		void OpenGLTexture2D::SetData(void* data, uint32_t size)
		{
			CEE_PROFILE_FUNCTION();
			
			uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
			CEE_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture");
			glTextureSubImage2D(m_RendererId, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
		}
		
		void OpenGLTexture2D::Bind(uint32_t slot) const
		{
			CEE_PROFILE_FUNCTION();
			
			glBindTexture(slot, m_RendererId);
		}
	}
}
