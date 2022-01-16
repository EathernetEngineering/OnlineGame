#pragma once
#include <Engine/Base.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace cee
{
	namespace engine
	{
		class Log
		{
		public:
			static void Init();
			
			static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
			static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
			
		private:
			static std::shared_ptr<spdlog::logger> s_CoreLogger;
			static std::shared_ptr<spdlog::logger> s_ClientLogger;
		};
	}
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::qua<T, Q>& quaternion)
{
	return os << glm::to_string(quaternion);
}

#define CEE_CORE_TRACE(...) ::cee::engine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CEE_CORE_INFO(...) ::cee::engine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CEE_CORE_WARN(...) ::cee::engine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CEE_CORE_ERROR(...) ::cee::engine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CEE_CORE_CRITICAL(...) ::cee::engine::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define CEE_TRACE(...) ::cee::engine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CEE_INFO(...) ::cee::engine::Log::GetClientLogger()->info(__VA_ARGS__)
#define CEE_WARN(...) ::cee::engine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CEE_ERROR(...) ::cee::engine::Log::GetClientLogger()->error(__VA_ARGS__)
#define CEE_CRITICAL(...) ::cee::engine::Log::GetClientLogger()->critical(__VA_ARGS__)


