#pragma once

#include <Engine/Event.h>
#include <Engine/Camera.h>
#include <Engine/Framebuffer.h>

namespace cee
{
	namespace engine
	{
		class OrthographicCameraController
		{
		public:
			OrthographicCameraController(std::shared_ptr< cee::engine::Framebuffer > framebuffer, bool rotation = false);
			
			void OnUpdate();
			void OnEvent(Event& e);
			
			void OnResize(float width, float height);
			
			OrthographicCamera& GetCamera() { return m_Camera; }
			const OrthographicCamera& GetCamera() const { return m_Camera; }
			
			float GetZoomLevel() const { return m_ZoomLevel; }
			void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; }
			
			float GetCameraTranslationSpeed() const { return m_CameraTranslationSpeed; };
			void SetCameraTranslationSpeed(float speed) { m_CameraTranslationSpeed = speed; }
			
			float GetCameraRotationSpeed() const { return m_CameraRotationSpeed; };
			void SetCameraRotationSpeed(float speed) { m_CameraRotationSpeed = speed; }
			
		private:
			bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
			bool OnMouseScrolled(MouseScrolledEvent& e);
			bool OnWindowResized(WindowResizeEvent& e);
			
		private:
			std::shared_ptr<Framebuffer> m_Framebuffer;
			glm::vec2 m_FramebufferSize;
			
			float m_AspectRatio;
			float m_ZoomLevel = 1.0f;
			OrthographicCamera m_Camera;
			
			glm::vec2 m_MousePosition;
			
			bool m_Rotation;
			
			glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
			float m_CameraRotation = 0.0f;
			float m_CameraTranslationSpeed = 1.0f, m_CameraRotationSpeed = 90.0f;
			glm::mat4 m_CameraTransformScaleMatrix;
		};
	}
}
