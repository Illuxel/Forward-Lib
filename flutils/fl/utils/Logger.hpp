#pragma once

#include "fl/utils/Memory.hpp"
#include "fl/utils/Flags.hpp"

#include "fl/utils/DateTime.hpp"

#include <chrono>
#include <fstream>

#include <queue>
#include <shared_mutex>

namespace Forward {
	
	class Logger : std::enable_shared_from_this<Logger>
	{
	public:
		enum class Level : uint8_t
		{
			INFO = 		0,	
			DEBUG = 	1 << 0,
			WARN = 		1 << 1,
			ERR = 		1 << 2,
			CRITICAL = 	1 << 3,
			TRACE = 	1 << 4,
		};

		enum class OutputFlag : uint8_t
		{
			None,
			File = 1 << 0,		// flag enables writing to file
			Console = 1 << 1	// flag enables printing to console
		};
		ENUM_FLAGS(OutputFlag, OutputFlags);

		struct Message
		{
			Logger::Level Level;
			std::string Data, Caller;
			DateTime Time;
		};

	private:
		std::string file_name_, time_format_, msg_format_;

		OutputFlags output_flags_;

		std::ofstream file_;
		Ref<std::queue <Message>> msg_queue_;

		mutable std::mutex mutex_;

	public:
		static constexpr inline const char* default_msg_format = "[%t] %l: %m";
		static constexpr inline const char* default_file_name = "log-dd-MM-hh-mm.txt";

	public:
		Logger();
		Logger(std::string_view file_name, Logger::OutputFlags flags);
		Logger(std::string_view file_name, 
			std::string_view msg_format, 
			std::string_view time_format, 
			Logger::OutputFlags flags);
		
		~Logger();

		void SetOutputMode(Logger::OutputFlags mode);
		/**
		 *	Set file name
		 *	
		 *	You can use datetime format to a file name.
		 * 	You can insert name with path and extension ONLY	
		 */
		void SetFileName(std::string_view file_name);
		/**
         *   HH    - 12-hour format
         *   hh    - 24-hour format
         *   mm    - minutes (0-59)
         *   ss    - seconds (0-59)
         *   MM    - month (1-12)
         *   dd    - day (1-31)
         *   yyyy  - year (e.g., 2021)
         */
		void SetDateTimeFormat(std::string_view time_format);
		/**
		 *	%t - appends time 
		 *	%c - appends caller
		 *	%m - appends log message
		 *	%l - appends log level
		 */
		void SetMessageFormat(std::string_view msg_format);

		// void BeginTrace(std::string_view func_name);
		// void EndTrace(std::string_view func_name);

		Message& FirstLog() const;
		Message& LastLog() const;

		void ConsoleOut(std::string_view data);
		void WriteInFile(std::string_view data);

		template<typename ...Args>
		void Log(Args&&... args) 
		{
			std::lock_guard<std::mutex> lock(mutex_);
			Message msg;

			msg.Time = DateTime::Now();
			
			PushMessage(msg);
			LogImplHelper(std::forward<Args>(args)...);
		}

		void PushMessage(Message const& msg);

	private:
		void LogImpl(Logger::Level level);
		void LogImpl(char const* data);
		void LogImpl(std::string const& data);

		void LogImplHelper();

		template<typename T, typename... Args>
		void LogImplHelper(T&& arg, Args&&... args)
		{
			LogImpl(std::forward<T>(arg));
			LogImplHelper(std::forward<Args>(args)...);
		}

		std::string ParseMessageFormat();
	};

	DECLARE_ENUM_OPERATORS(Logger::OutputFlag);
}