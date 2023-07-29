#pragma once

#include "fl/utils/Logger.hpp"
#include "fl/utils/Memory.hpp"

#include <unordered_map>

namespace Forward {

	class Log
	{
    private:
		std::shared_mutex pool_mtx_;
		std::unordered_map<std::string, Ref<Logger>> pool_;

	public:
		static Ref<Logger> Get(std::string_view log_name = "");

		static void Init(std::string_view log_name = "");
		static void Remove(std::string_view log_name = "");
	
		Log(Log&&) = delete;
		Log(Log const&) = delete;

	private: 
		Log();

		static Log& Instance();
	};
}

template<typename T>
void PrintInfo(std::string const& call, T const& msg) 
{
	std::cout << msg << std::endl;
}

#define FL_LOG(call, msg) \
	PrintInfo(call, msg)

#define FL_ERR(call, msg) \
	PrintInfo(call, msg); \
	throw Exception(msg)

	//Forward::Log::Get()->Log(Forward::Logger::Level::level, __FUNCTION__, __VA_ARGS__); 

// #define FL_INFO(...) FL_LOG(INFO, __VA_ARGS__)
// #define FL_DEBUG(...) FL_LOG(DEBUG, __VA_ARGS__)
// #define FL_WARN(...) FL_LOG(WARN, __VA_ARGS__)
// #define FL_LOG(...) FL_LOG(ERR, __VA_ARGS__)
// #define FL_CRITICAL(...) FL_LOG(CRITICAL, __VA_ARGS__)