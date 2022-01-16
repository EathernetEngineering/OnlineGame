#pragma once
#include <string>

#include <Engine/Event.h>

namespace cee
{
	namespace engine
	{
		class Layer
		{
		public:
			Layer(const std::string& debugName);
			virtual ~Layer() = default;
			
		public:
			virtual void OnAttach() = 0;
			virtual void OnDetach() = 0;
			
			virtual void OnEvent(Event& e) = 0;
			virtual void OnUpdate() = 0;
			virtual void OnLateUpdate() = 0;
			virtual void OnFixedUpdate() = 0;
			virtual void OnRender() = 0;
			virtual void OnImGuiRender() = 0;
			
		private:
			std::string m_DebugName;
		};
	}
}
