#include <Engine/pch.h>
#include <Engine/OrthographicCameraController.h>

#include <Engine/Input.h>

#include <Engine/Application.h>

namespace cee
{
	namespace engine
	{
		OrthographicCameraController::OrthographicCameraController(std::shared_ptr<Framebuffer> framebuffer, bool rotation)
			: m_Framebuffer(framebuffer),
			  m_FramebufferSize(m_Framebuffer->GetSpecification().Width, m_Framebuffer->GetSpecification().Width),
			  m_AspectRatio(framebuffer->GetSpecification().Width/framebuffer->GetSpecification().Height),
			  m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
			  m_Rotation(rotation)
		{
			m_MousePosition = Input::GetMousePos();
			m_CameraTransformScaleMatrix = glm::ortho(0.0f, m_FramebufferSize.x, 0.0f, m_FramebufferSize.y, -m_ZoomLevel, m_ZoomLevel);
		}
		
		void OrthographicCameraController::OnUpdate()
		{
			CEE_PROFILE_FUNCTION();
			
			if (cee::engine::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_FramebufferSize.x > 0.0f && m_FramebufferSize.y > 0.0f &&
			(spec.Width != m_FramebufferSize.x || spec.Height != m_FramebufferSize.y))
			{
				OnResize(spec.Width, spec.Height);
			}
			
			if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
			{
				glm::vec4 newMousePos = glm::vec4(Input::GetMousePos(), 0.0, 1.0);
				glm::vec4 mousePosition4 = glm::vec4(m_MousePosition, 0.0f, 1.0f);
				
				newMousePos = glm::ortho(0.0f, m_FramebufferSize.x, 0.0f, m_FramebufferSize.y, -m_ZoomLevel, m_ZoomLevel) * newMousePos;
				mousePosition4 = glm::ortho(0.0f, m_FramebufferSize.x, 0.0f, m_FramebufferSize.y, -m_ZoomLevel, m_ZoomLevel) * mousePosition4;
				
				glm::vec2 movedDistance = glm::vec2(newMousePos - mousePosition4);
				m_CameraPosition.x -= m_CameraTranslationSpeed * movedDistance.x;
				m_CameraPosition.y += m_CameraTranslationSpeed * movedDistance.y;
				
				m_MousePosition = Input::GetMousePos();
			}
			
			if (m_Rotation)
			{
				if(Input::IsKeyPressed(Key::Q))
					m_CameraRotation += m_CameraRotationSpeed * Application::Get().GetPreviousFrameTime() * 0.001f;
				if(Input::IsKeyPressed(Key::E))
					m_CameraRotation -= m_CameraRotationSpeed * Application::Get().GetPreviousFrameTime() * 0.001f;
				
				if (m_CameraRotation > 180.0f)
					m_CameraRotation -= 360.0f;
				else if (m_CameraRotation < -180.0f)
					m_CameraRotation += 360.0f;
				
				m_Camera.SetRotation(m_CameraRotation);
			}
			m_Camera.SetPosition(m_CameraPosition);
		}
		
		void OrthographicCameraController::OnEvent(Event& e)
		{
			CEE_PROFILE_FUNCTION();
			
			EventDispatcher d(e);
			d.Dispatch<MouseButtonPressedEvent>(CEE_BIND_EVENT_FN(OnMouseButtonPressed));
			d.Dispatch<MouseScrolledEvent>(CEE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
			d.Dispatch<WindowResizeEvent>(CEE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
		}
		
		void OrthographicCameraController::OnResize(float width, float height)
		{
			m_AspectRatio = width/height;
			m_FramebufferSize.x = width;
			m_FramebufferSize.y = height;
			m_CameraTransformScaleMatrix = glm::ortho(0.0f, m_FramebufferSize.x, 0.0f, m_FramebufferSize.y, -m_ZoomLevel, m_ZoomLevel);
			m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		}
		
		bool OrthographicCameraController::OnMouseButtonPressed(MouseButtonPressedEvent& e)
		{
			m_MousePosition = Input::GetMousePos();
			return true;
		}
		
		bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
		{
			CEE_PROFILE_FUNCTION();
			
			m_ZoomLevel -= e.GetYOffset() * 0.25f;
			m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
			m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
			return true;
		}
		
		bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
		{
			CEE_PROFILE_FUNCTION();
			
			OnResize((float)e.GetWidth(), (float)e.GetHeight());
			return true;
		}
	}
}
