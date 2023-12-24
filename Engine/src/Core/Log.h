#pragma once
#pragma once

#include "Base.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)



class Log
{
public:
	static void Initialize();

	static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

private:
	static Ref<spdlog::logger> s_CoreLogger;
	static Ref<spdlog::logger> s_ClientLogger;
};

#ifndef DISTRIBUTION

	#define CORE_TRACE(...)    ::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define CORE_INFO(...)     ::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define CORE_WARN(...)     ::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define CORE_ERROR(...)    ::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define CORE_CRITICAL(...) ::Log::GetCoreLogger()->critical(__VA_ARGS__)
								 
	#define TRACE(...)         ::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define INFO(...)          ::Log::GetClientLogger()->info(__VA_ARGS__)
	#define WARN(...)          ::Log::GetClientLogger()->warn(__VA_ARGS__)
	//#define ERROR(...)         ::Log::GetClientLogger()->error(__VA_ARGS__)
	#define CRITICAL(...)      ::Log::GetClientLogger()->critical(__VA_ARGS__)
	
	#define CORE_VERIFY(x, ...) if (!(x)) CORE_CRITICAL(__VA_ARGS__)
	#define VERIFY(x, ...)      if (!(x)) CRITICAL(__VA_ARGS__)

#else

	#define CORE_TRACE(...)	     (void*)0;
	#define CORE_INFO(...)		 (void*)0;
	#define CORE_WARN(...)		 (void*)0;
	#define CORE_ERROR(...)		 (void*)0;
	#define CORE_CRITICAL(...)   (void*)0;

	#define TRACE(...)           (void*)0;
	#define INFO(...)			 (void*)0;
	#define WARN(...)			 (void*)0;
	//#define ERROR(...)			 (void*)0;
	#define CRITICAL(...)		 (void*)0;
	
	#define CORE_VERIFY(x, ...)  (void*)0;
	#define VERIFY(x, ...) 		 (void*)0;

#endif