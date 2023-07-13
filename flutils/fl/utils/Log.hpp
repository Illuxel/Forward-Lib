#pragma once

#include "fl/utils/Memory.hpp"
#include "fl/utils/Logger.hpp"

#include <iostream>
#include <unordered_map>

namespace Forward {

	class Log
	{
    private:
		static std::unordered_map<std::string, Ref<Logger>> logs_;

	public:
		Log(Log const&) = delete;
		Log(Log&&) = delete;

		static Ref<Logger> Get(std::string_view name = "");

		static void Innit(std::string_view name = "");
		static void Remove(std::string_view name);
	
	};
}

void printInfo(std::string const& call, std::string const& msg);

#define FL_LOG(call, msg) \
	printInfo(call, msg)

#define FL_ERR(call, msg) \
	printInfo(call, msg); \
	throw Exception(msg)

	//Forward::Log::Get()->Log(Forward::Logger::Level::level, __FUNCTION__, __VA_ARGS__); 

// #define FL_INFO(...) FL_LOG(INFO, __VA_ARGS__)
// #define FL_DEBUG(...) FL_LOG(DEBUG, __VA_ARGS__)
// #define FL_WARN(...) FL_LOG(WARN, __VA_ARGS__)
// #define FL_LOG(...) FL_LOG(ERR, __VA_ARGS__)
// #define FL_CRITICAL(...) FL_LOG(CRITICAL, __VA_ARGS__)