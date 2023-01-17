#pragma once

#include <QObject>

#include <QDateTime>

#include <fstream>
#include <queue>

namespace fl::utils {
	
	class StringArg;

	class Logger
		: public QObject
	{
	public:
		Logger(QObject* parent = nullptr);
		Logger(std::string_view fileName, QObject* parent = nullptr);
		~Logger() override;

		enum Level
		{
			TRACE = 0,	// flag encapsulates function log
			INFO,		// flag useful information
			DEBUG,		// flag for dev only
			WARN,		// flag not affected to a program
			ERROR,		// flag
			CRITICAL	// flag
		};
		Q_ENUM(Level);

		enum OutputMode
		{
			Write = 7,		// flag enables writing to file
			Print,		// flag enables printing to console

			PrintWrite = Print | Write // flag enables all
		};
		Q_DECLARE_FLAGS(OutputModeFlags, OutputMode);

		struct Message
		{
			Logger::Level Level;

			QDateTime 	DateTime;
			QObject* 	Sender;

			std::string Info;

			Message& operator<<(const char* msg);
			friend QDebug operator<<(QDebug os, const Message& msg);
		};

		void SetOutputMode(Logger::OutputModeFlags mode);
		/**
		 *	dd 		- day
		 *	MM 		- month
		 *	yyyy 	- year
		 *	mm 		- minutes
		 *	h		- hours
		 *	m		- minutes
		 *	s		- seconds
		 *
		 *	doubled - enables 24 time format
		 */
		void SetDateTimeFormat(std::string_view format);
		/**
		 *	%i - appends log info
		 *	%c - appends object
		 *	%t - appends time
		 */
		void SetMessagePattern(std::string_view msgPattern);
		void SetFileNamePattern(std::string_view filePattern);

	private:
		void CreateFile();

		std::string_view ParseFileNameFormat() const;
		std::string_view ParseMessageFormat() const;

		std::string m_ParsedFileName, m_ParsedMessageFormat;

	public:
		const std::queue <Message>& GetQuery() const 
		{
			return m_LogMessages;
		}

		void StartTrace();
		void EndTrace();

		Logger::Message& log(Logger::Level level, QObject* object = nullptr);

	private:
		// Logger states
		bool isDate, isObject, isReason,
			toFile, toConsole;

		std::string m_TimeFormat, m_LogMessagePattern, m_FileNamePattern;

		std::fstream m_LogFile;

		std::queue <Message> m_LogMessages;

		const std::vector <StringArg> m_FormatArgs;
	};

	Q_DECLARE_OPERATORS_FOR_FLAGS(Logger::OutputModeFlags);
}