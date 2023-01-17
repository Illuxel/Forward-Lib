#pragma once

#include <iostream>

#include <unordered_map>

namespace fl::utils {

	class Logger;

	class Log
	{
		explicit Log();

	public:
		Log(const Log&) = delete;
		Log(Log&&) = delete;

		static std::Logger* Get(const std::string& name = "");

		static void Innit(const std::string& name = "");
		static void Remove(const std::string& name);

		~Log();

	private:
	
		static std::unordered_map<std::string, Logger> s_logPool;
	};
}

#ifdef _DEBUG
#define FL_DEBUG (...) \
	// std::cout << ""
// #else
// #define ALO_DEBUG (...)
#endif

#define FL_TRACE_BEGIN() \

#define FL_TRACE_END() \

#define FL_LOG(level, ...) \
	Log::Get()->log(Logger::Level::level)
	