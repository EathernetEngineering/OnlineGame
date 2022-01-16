#pragma once

namespace cee
{
	namespace engine
	{
		class Renderer
		{
		public:
			virtual ~Renderer() = default;
			
			static void Init();
		};
	}
}
