#include "fl/utils/Logger.hpp"

#include "fl/utils/StringArgParser.hpp"
#include "fl/utils/StringBuilder.hpp"

#include <iostream>
#include <future>

namespace Forward {

	static std::string FromEnumToString(Logger::Level level)
	{
		switch (level)
		{
		case Logger::Level::INFO:
			return "INFO";
		case Logger::Level::DEBUG:
			return "DEBUG";
		case Logger::Level::WARN:
			return "WARN";
		case Logger::Level::TRACE:
			return "TRACE";
		case Logger::Level::ERR:
			return "ERROR";
		case Logger::Level::CRITICAL:
			return "CRITICAL";
		default:
			return "";
		}
	}

	Logger::Logger()
	{
		SetOutputMode(OutputFlag::Console | OutputFlag::File);

		SetFileName(DefaultFileFormat);
		SetDateTimeFormat(DateTime::DefaultFormat);
		SetMessageFormat(DefaultMsgFormat);
	}
	Logger::Logger(std::string_view file_name, Logger::OutputFlags mode)
		: out_mode_(mode)
	{
		SetFileName(file_name);
		SetDateTimeFormat(DateTime::DefaultFormat);
		SetMessageFormat(DefaultMsgFormat);
	}
	Logger::Logger(std::string_view file_name, Logger::OutputFlags mode, std::string_view msg_format, std::string_view time_format)
		: out_mode_(mode)
	{
		SetFileName(file_name);
		SetMessageFormat(msg_format);
		SetDateTimeFormat(time_format);
	}

    Logger::~Logger()
    {
		file_.close();
    }

	void Logger::SetOutputMode(Logger::OutputFlags mode)
	{
		out_mode_ = mode;
	}

	void Logger::SetFileName(std::string_view file_name)
	{
		file_name_ = file_name;
	}
	void Logger::SetDateTimeFormat(std::string_view time_format)
	{
		time_format_ = time_format;
	}
	void Logger::SetMessageFormat(std::string_view msg_format)
	{
		msg_format_ = msg_format;
	}

	Logger::Message& Logger::FirstLog() &
	{
		std::shared_lock lock(queue_mtx_);
		return msg_queue_.back();
	}
	Logger::Message const& Logger::FirstLog() const&
	{
		std::shared_lock lock(queue_mtx_);
		return msg_queue_.back();
	}

	Logger::Message& Logger::LastLog() &
	{
		std::shared_lock lock(queue_mtx_);
		return msg_queue_.front();
	}
	Logger::Message const& Logger::LastLog() const&
	{
		std::shared_lock lock(queue_mtx_);
		return msg_queue_.front();
	}

	void Logger::PushMessage(Logger::Message const& msg) 
	{
		std::unique_lock lock(queue_mtx_);
		msg_queue_.push(msg);
	}

	void Logger::LogImplHelper() 
	{
		auto parsed_msg = ParseMessageFormat();
		
		//std::async(std::launch::async, [this](std::string parsed_msg) {
		//	this->ConsoleOut(parsed_msg);
		//}, parsed_msg);

		//std::async(std::launch::async, [this](std::string parsed_msg) {
		//	this->WriteInFile(parsed_msg);
		//}, parsed_msg);
	}
	
	void Logger::LogImpl(Logger::Level level) 
	{
		auto& msg = LastLog();
		msg.Level = level;
	}
	void Logger::LogImpl(char const* data) 
	{
		LogImpl(std::string(data));
	}
	void Logger::LogImpl(std::string const& data)
	{
		auto& msg = LastLog();

		if (msg.Caller.empty())
		{
			msg.Caller = data;
			return;
		}

		msg.Data += data + ' ';
	}

	/**
	 *	%t - appends time
	 *	%c - appends caller
	 *	%m - appends log message
	 *	%l - appends log level
	 */
	std::string Logger::ParseMessageFormat()
	{
		Message const& msg = LastLog();
		
		auto msg_format_args = 
		StringArg::MakeArgs(
		{
			{ "t", msg.Time.ToString() },
			{ "c", msg.Caller },
			{ "m", msg.Data	},
			{ "l", FromEnumToString(msg.Level) }
		});

		return StringBuilder(msg_format_, msg_format_args);
	}

	void Logger::ConsoleOut(std::string_view data) 
	{
		if (!out_mode_.Test(OutputFlag::Console))
			return;

		std::unique_lock lock(io_mtx_);
		std::cout << data << std::endl;
	}
	void Logger::WriteInFile(std::string_view data)
	{
		if (!out_mode_.Test(OutputFlag::File))
			return;

		std::unique_lock lock(io_mtx_);

		if (!file_.is_open())
		{
			file_.open(DateTime::Now().ToString(file_name_));

			if (!file_.is_open())
			{
				std::cout << "Log file could not be opened" << std::endl;
				return;
			}
		}

		file_ << data.data() << std::endl;
	}
}