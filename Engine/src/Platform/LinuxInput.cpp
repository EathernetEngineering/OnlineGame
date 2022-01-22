#include <Engine/pch.h>
#include <Engine/Input.h>

#include <Engine/Application.h>
#include <GLFW/glfw3.h>

namespace cee
{
	namespace engine
	{
		bool Input::IsKeyPressed(KeyCode key)
		{
			auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindowPtr());
			auto state = glfwGetKey(window, static_cast<int32_t>(key));
			return state == GLFW_PRESS || state == GLFW_REPEAT;
		}
		
		bool Input::IsMouseButtonPressed(MouseCode button)
		{
			auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindowPtr());
			auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
			return state == GLFW_PRESS;
		}
		
		glm::vec2 Input::GetMousePos()
		{
			auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindowPtr());
			double xPos, yPos;
			glfwGetCursorPos(window, &xPos, &yPos);
			
			return { (float)xPos, (float)yPos };
		}
		
		float Input::GetMouseX()
		{
			return Input::GetMousePos().y;
		}
		
		float Input::GetMouseY()
		{
			return Input::GetMousePos().x;
		}
	}
}
