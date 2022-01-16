#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <deque>
#include <optional>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <condition_variable>
#include <functional>

#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>

#include <boost/system/system_error.hpp>

namespace cee
{
	namespace net
	{
		enum class ErrorSeverity : uint8_t
		{
			TRACE = 0, INFO, WARN, ERROR, CRITICAL
		};
		
		void SetErrorCallback(void(ErrorSeverity, const std::string&));
		
		extern std::function<void(ErrorSeverity, const std::string&)> callbackFn;
		
		void HandleError(ErrorSeverity severity, const std::string& message);
	}
}
