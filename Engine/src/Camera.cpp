#include <Engine/pch.h>
#include <Engine/Camera.h>

namespace cee
{
	namespace engine
	{
		OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
			: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(glm::identity<glm::mat4>())
		{
			CEE_PROFILE_FUNCTION();
			
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
		
		void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
		{
			CEE_PROFILE_FUNCTION();
			
			m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
		
		void OrthographicCamera::RecalculateViewMatrix()
		{
			CEE_PROFILE_FUNCTION();
			
			glm::mat4 transform = glm::translate(glm::identity<glm::mat4>(), m_Position) * glm::rotate(glm::identity<glm::mat4>(), glm::radians(m_Rotation), { 0, 0, 1 });
			
			m_ViewMatrix = glm::inverse(transform);
			
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
	}
}
