#include <common/Common.hpp>

namespace cee
{
	namespace net
	{
		std::function<void(ErrorSeverity, const std::string&)> callbackFn;
		
		void HandleError(ErrorSeverity severity, const std::string& message)
			{
				if (callbackFn)
				{
					callbackFn(severity, message);
				}
			}
		
		void SetErrorCallback(void(*callback)(ErrorSeverity, const std::string&))
		{
			callbackFn = callback;
		}
	}
}
