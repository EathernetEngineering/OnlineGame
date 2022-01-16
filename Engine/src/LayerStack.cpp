#include <Engine/pch.h>
#include <Engine/LayerStack.h>
#include <algorithm>

namespace cee
{
	namespace engine
	{
		LayerStack::~LayerStack()
		{
			for (Layer* layer : m_Stack)
			{
				layer->OnDetach();
				delete layer;
			}
		}
		
		void LayerStack::PushLayer(Layer* layer)
		{
			m_Stack.emplace(m_Stack.begin() + m_StackInsertIndex, layer);
			m_StackInsertIndex++;
		}
		
		void LayerStack::PopLayer(Layer* layer)
		{
			auto it = std::find(m_Stack.begin(), m_Stack.begin() + m_StackInsertIndex, layer);
			if (it != m_Stack.begin() + m_StackInsertIndex)
			{
				layer->OnDetach();
				m_Stack.erase(it);
				m_StackInsertIndex--;
			}
		}
		
		void LayerStack::PushOverlay(Layer* overlay)
		{
			m_Stack.emplace_back(overlay);
		}
		
		void LayerStack::PopOverlay(Layer* overlay)
		{
			auto it = std::find(m_Stack.begin(), m_Stack.begin() + m_StackInsertIndex, overlay);
			if (it != m_Stack.end())
			{
				overlay->OnDetach();
				m_Stack.erase(it);
			}
		}
	}
}
