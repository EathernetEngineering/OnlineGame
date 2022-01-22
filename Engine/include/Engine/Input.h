#pragma once

#include <glm/glm.hpp>

#include <Engine/KeyCodes.h>

namespace cee
{
	namespace engine
	{
		class Input
		{
		public:
			static bool IsKeyPressed(KeyCode key);
			static bool IsMouseButtonPressed(MouseCode button);
			
			static glm::vec2 GetMousePos();
			static float GetMouseX();
			static float GetMouseY();
		};
	}
}
