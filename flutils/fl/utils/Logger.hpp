<<<<<<< Updated upstream
#pragma once

#include "fl/utils/Flags.hpp"
#include "fl/utils/DateTime.hpp"

#include "fl/utils/LoggerStream.hpp"

#include <queue>

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
			DateTime Time;
			Logger::Level Level;
			std::string Caller, Data;
		};

	private:
		std::string file_name_, time_format_, msg_format_;

		OutputFlags out_mode_;

		std::ofstream file_;
		std::queue<Message> msg_queue_;

		mutable std::shared_mutex io_mtx_, queue_mtx_;

	public:
		static constexpr inline const char* DefaultMsgFormat = "%t %l: %m";
		static constexpr inline const char* DefaultFileFormat = "log-dd-MM-hh-mm.txt";

	public:
		Logger();
		Logger(std::string_view file_name, Logger::OutputFlags flags);
		Logger(std::string_view file_name, Logger::OutputFlags flags,
			std::string_view msg_format, 
			std::string_view time_format);
		
		~Logger();


		void SetOutputMode(Logger::OutputFlags mode);

		/**
		 *	Sets file name format of outputted log file. Use datetime format 
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

		Message& FirstLog() &;
		Message const& FirstLog() const&;

		Message& LastLog() &;
		Message const& LastLog() const&;

		void ConsoleOut(std::string_view data);
		void WriteInFile(std::string_view data);

		template<typename ...Args>
		void Log(Args&&... args) 
		{
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
=======
#pragma once

#include "fl/utils/Flags.hpp"
#include "fl/utils/DateTime.hpp"

#include "fl/utils/LoggerStream.hpp"

#include <queue>

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
			DateTime Time;
			Logger::Level Level;
			std::string Caller, Data;
		};

	private:
		std::string file_name_, time_format_, msg_format_;

		OutputFlags out_mode_;

		std::ofstream file_;
		std::queue<Message> msg_queue_;

		mutable std::shared_mutex io_mtx_, queue_mtx_;

	public:
		static constexpr inline const char* DefaultMsgFormat = "%t %l: %m";
		static constexpr inline const char* DefaultFileFormat = "log-dd-MM-hh-mm.txt";

	public:
		Logger();
		Logger(std::string_view file_name, Logger::OutputFlags flags);
		Logger(std::string_view file_name, Logger::OutputFlags flags,
			std::string_view msg_format, 
			std::string_view time_format);
		
		~Logger();


		void SetOutputMode(Logger::OutputFlags mode);

		/**
		 *	Sets file name format of outputted log file. Use datetime format 
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

		Message& FirstLog() &;
		Message const& FirstLog() const&;

		Message& LastLog() &;
		Message const& LastLog() const&;

		void ConsoleOut(std::string_view data);
		void WriteInFile(std::string_view data);

		template<typename ...Args>
		void Log(Args&&... args) 
		{
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
>>>>>>> Stashed changes
