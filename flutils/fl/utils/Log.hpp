#pragma once

#include <unordered_map>

#include "fl/utils/Memory.hpp"
#include "fl/utils/Logger.hpp"

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
	
		static std::vector <std::string> LogNames();

		~Log();

		Log(Log&&) = delete;
		Log(Log const&) = delete;

	private: 
		explicit Log();

		static Log& Instance();
	};
}

template<typename T>
void PrintInfo(std::string const& caller, T const& msg) 
{
	static std::mutex out_mtx_;

	std::lock_guard lock(out_mtx_);
	std::cout << caller << ": " << msg << std::endl;
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