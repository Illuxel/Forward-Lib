#pragma once

#include "fl/utils/Memory.hpp"
#include "fl/utils/Logger.hpp"

#include <iostream>
#include <unordered_map>

namespace fl::utils {

	class Log
	{
		static std::unordered_map<std::string, Ref<Logger>> logs_;

	public:
		Log(Log const&) = delete;
		Log(Log&&) = delete;

		static Ref<Logger> Get(std::string_view name = "");

		static void Innit(std::string_view name = "");
		static void Remove(std::string_view name);
	
	};
}

static inline void printInfo(std::string_view call, std::string_view msg)
{
	std::cout << call << ": " << msg << std::endl;
}

#define FL_LOG(call, msg) \
	printInfo(call, msg)

	//fl::utils::Log::Get()->Log(fl::utils::Logger::Level::level, __FUNCTION__, __VA_ARGS__); 

// #define FL_INFO(...) FL_LOG(INFO, __VA_ARGS__)
// #define FL_DEBUG(...) FL_LOG(DEBUG, __VA_ARGS__)
// #define FL_WARN(...) FL_LOG(WARN, __VA_ARGS__)
// #define FL_LOG(...) FL_LOG(ERR, __VA_ARGS__)
// #define FL_CRITICAL(...) FL_LOG(CRITICAL, __VA_ARGS__)