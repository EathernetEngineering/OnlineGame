#pragma once

#include <Engine/Layer.h>

namespace cee
{
	namespace engine
	{
		class ImGuiLayer : public Layer
		{
		public:
			ImGuiLayer();
			virtual ~ImGuiLayer() {};
			
			virtual void OnAttach() override;
			virtual void OnDetach() override;
			
			void Begin();
			void End();
			
			virtual void OnEvent(Event& e) override;
			virtual void OnUpdate() override;
			virtual void OnImGuiRender() override;
		};
	}
}
