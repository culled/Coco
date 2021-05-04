#pragma once

#include "Core/Base.h"
#include "spdlog/spdlog.h"

namespace Coco
{
	class COCO_API Log
	{
	public:
		/*@brief Initializes the logger*/
		static void Init();

		/*@brief Gets the core logging system*/
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

		/*@brief Gets the client (application) logging system*/
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

	//Core log macros
	/*@brief Logs a fatal error to the core logger*/
	#define LOG_CORE_FATAL(...) ::Coco::Log::GetCoreLogger()->fatal(__VA_ARGS__)
	/*@brief Logs an error to the core logger*/
	#define LOG_CORE_ERROR(...) ::Coco::Log::GetCoreLogger()->error(__VA_ARGS__)
	/*@brief Logs a warning to the core logger*/
	#define LOG_CORE_WARN(...)  ::Coco::Log::GetCoreLogger()->warn(__VA_ARGS__)
	/*@brief Logs info to the core logger*/
	#define LOG_CORE_INFO(...)  ::Coco::Log::GetCoreLogger()->info(__VA_ARGS__)
	/*@brief Logs a trace to the core logger*/
	#define LOG_CORE_TRACE(...) ::Coco::Log::GetCoreLogger()->trace(__VA_ARGS__)

	//Client log macros
	/*@brief Logs a fatal error to the client logger*/
	#define LOG_FATAL(...) ::Coco::Log::GetClientLogger()->fatal(__VA_ARGS__)
	/*@brief Logs an error to the client logger*/
	#define LOG_ERROR(...) ::Coco::Log::GetClientLogger()->error(__VA_ARGS__)
	/*@brief Logs a warning to the client logger*/
	#define LOG_WARN(...)  ::Coco::Log::GetClientLogger()->warn(__VA_ARGS__)
	/*@brief Logs info to the client logger*/
	#define LOG_INFO(...)  ::Coco::Log::GetClientLogger()->info(__VA_ARGS__)
	/*@brief Logs a trace to the client logger*/
	#define LOG_TRACE(...) ::Coco::Log::GetClientLogger()->trace(__VA_ARGS__)
}

