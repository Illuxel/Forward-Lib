#include "fl/utils/Logger.hpp"
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

		SetFileName(default_file_name);
		SetDateTimeFormat(default_time_format);
		SetMessageFormat(default_msg_format);
	}
	Logger::Logger(std::string_view file_name, Logger::OutputFlags flags)
		: output_flags_(flags)
	{
		SetFileName(file_name);

		SetDateTimeFormat(default_time_format);
		SetMessageFormat(default_msg_format);
	}
	Logger::Logger(std::string_view file_name, std::string_view msg_format, std::string_view time_format, Logger::OutputFlags flags)
		: output_flags_(flags)
	{
		SetFileName(file_name);
		SetMessageFormat(msg_format);
		SetDateTimeFormat(time_format);
	}

    Logger::~Logger()
    {
		msg_queue_.reset();
		file_.close();
    }

	void Logger::SetOutputMode(Logger::OutputFlags output_mode)
	{
		output_flags_ = output_mode;
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

	Logger::Message& Logger::FirstLog() const
	{
		return msg_queue_->back();
	}
	Logger::Message& Logger::LastLog() const
	{
		return msg_queue_->front();
	}

	void Logger::PushMessage(Logger::Message const& msg) 
	{
		if (!msg_queue_)
			msg_queue_ = MakeRef<std::queue <Message>>();

		std::lock_guard<std::mutex> lock(mutex_);
		msg_queue_->push(msg);
	}

	void Logger::LogImplHelper() 
	{
		auto parsed_msg = ParseMessageFormat();
		
		std::future<void> console = std::async(std::launch::async, [this](std::string parsed_msg) {
			this->ConsoleOut(parsed_msg);
		}, parsed_msg);
		
		console.wait();

		std::future<void> write = std::async(std::launch::async, [this](std::string parsed_msg) {
			this->WriteInFile(parsed_msg);
		}, parsed_msg);
		
		write.wait();
	}
	
	void Logger::LogImpl(Logger::Level level) 
	{
		std::lock_guard<std::mutex> lock(mutex_);
		auto& msg = LastLog();
		msg.Level = level;
	}
	void Logger::LogImpl(char const* data) 
	{
		LogImpl(std::string(data));
	}
	void Logger::LogImpl(std::string const& data)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		auto& msg = LastLog();

		if (msg.Caller.empty()) 
		{
			msg.Caller = data;
			return;
		}

		msg.Data += data + ' ';
	}

	std::string Logger::ParseMessageFormat()
	{
		std::lock_guard<std::mutex> lock(mutex_);

		auto& msg = LastLog();
		/**
		 *	%t - appends time 
		 *	%c - appends caller
		 *	%m - appends log message
		 *	%l - appends log level
		 */
		auto msg_format_args = StringArg::MakeArgs({'t','c','m','l'}, true);
	
		msg_format_args[0].SetData(msg.Time.ToString()); 
		msg_format_args[1].SetData(msg.Caller); 
		msg_format_args[2].SetData(msg.Data); 
		msg_format_args[3].SetData(FromEnumToString(msg.Level)); 

		return StringBuilder(msg_format_, msg_format_args);
	}

	void Logger::ConsoleOut(std::string_view data) 
	{
		auto is_console = output_flags_.Test(OutputFlag::Console);

		if (!is_console)
			return;

		std::lock_guard<std::mutex> lock(mutex_);
		std::cout << data << std::endl;
	}
	void Logger::WriteInFile(std::string_view data)
	{
		auto is_file = output_flags_.Test(OutputFlag::File);

		if (!is_file)
			return;

		std::lock_guard<std::mutex> lock(mutex_);

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