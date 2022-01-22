#include <Engine/pch.h>
#include <Engine/Renderer2D.h>

#include <Engine/VertexArray.h>
#include <Engine/Shader.h>

#include <Engine/RenderCommand.h>

namespace cee
{
	namespace engine
	{
		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
		};
		
		struct Renderer2DData
		{
			static const uint32_t MaxQuads = 10000;
			static const uint32_t MaxVertices = MaxQuads * 4;
			static const uint32_t MaxIndices = MaxQuads * 6;
			
			std::shared_ptr<VertexArray> QuadVertexArray;
			std::shared_ptr<VertexBuffer> QuadVertexBuffer;
			std::shared_ptr<Shader> ColorShader;
			
			uint32_t QuadIndexCount;
			QuadVertex* QuadVertexBufferBase = nullptr;
			QuadVertex* QuadVertexBufferPtr = nullptr;
			
			glm::vec4 QuadVertexPositions[4];
			
			Renderer2D::Statistics Stats;
			
			struct CameraData
			{
				glm::mat4 ViewProjection;
			};
			CameraData CameraBuffer;
		};
		
		static Renderer2DData s_Data;
		
		void Renderer2D::Init()
		{
			CEE_PROFILE_FUNCTION();
			
			s_Data.QuadVertexArray = VertexArray::Create();
			
			s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
			s_Data.QuadVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "pos" },
				{ ShaderDataType::Float4, "color" }
			});
			s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
			
			s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
			uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
			
			uint32_t offset = 0;
			for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
			{
				quadIndices[i + 0] = offset + 0;
				quadIndices[i + 1] = offset + 1;
				quadIndices[i + 2] = offset + 2;
				
				quadIndices[i + 3] = offset + 2;
				quadIndices[i + 4] = offset + 3;
				quadIndices[i + 5] = offset + 0;
				
				offset += 4;
			}
			
			std::shared_ptr<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
			s_Data.QuadVertexArray->SetIndexBuffer(quadIndexBuffer);
			delete[] quadIndices;
			
			s_Data.ColorShader = Shader::Create("Engine/res/shaders/SimpleVertex.glsl", "Engine/res/shaders/SimpleFragment.glsl");
			
			s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
		}
		
		void Renderer2D::Shutdown()
		{
			CEE_PROFILE_FUNCTION();
			delete[] s_Data.QuadVertexBufferBase;
			s_Data.~Renderer2DData();
		}
		
		void Renderer2D::BeginScene(const OrthographicCamera& camera)
		{
			CEE_PROFILE_FUNCTION();
			
			s_Data.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
			Renderer2D::StartBatch();
		}
		
		void Renderer2D::EndScene()
		{
			CEE_PROFILE_FUNCTION();
			
			Renderer2D::Flush();
		}
		
		void Renderer2D::Flush()
		{
			if (s_Data.QuadIndexCount == 0)
				return; // Nothing to draw;
			
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
			
			s_Data.ColorShader->Bind();
			s_Data.QuadVertexArray->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
			s_Data.Stats.DrawCalls++;
		}
		
		void Renderer2D::DrawQuad(const glm::vec3 position, const glm::vec2 scale, const glm::vec4 color)
		{
			CEE_PROFILE_FUNCTION();
			
			constexpr size_t quadVertexCount = 4;
			
			if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
				NextBatch();
			
			glm::mat4 transform = glm::mat4(1.0f);
			
			transform = glm::scale(transform, glm::vec3(scale, 1.0f));
			transform = glm::translate(transform, position);
			transform = transform * s_Data.CameraBuffer.ViewProjection;
			
			for (size_t i = 0; i < quadVertexCount; i++)
			{
				s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
				s_Data.QuadVertexBufferPtr->Color = color;
				s_Data.QuadVertexBufferPtr++;
			}
			s_Data.QuadIndexCount += 6;
			s_Data.Stats.QuadCount++;
		}
		
		void Renderer2D::ResetStats()
		{
			memset(&s_Data.Stats, 0, sizeof(Renderer2D::Statistics));
		}
		
		Renderer2D::Statistics Renderer2D::GetStats()
		{
			return s_Data.Stats;
		}
		
		void Renderer2D::StartBatch()
		{
			s_Data.QuadIndexCount = 0;
			s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		}
		
		void Renderer2D::NextBatch()
		{
			Renderer2D::Flush();
			Renderer2D::StartBatch();
		}
	}
}
