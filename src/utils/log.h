#pragma once

#include <QObject>

#include <QFile>

#include <QMutex>
#include <QThread>

#include <QDateTime>

#include "aloAPI_global.h"

namespace alo::Utils {

	class ALO_API Logging
		: private QObject
	{
		Q_OBJECT

	public:
		

		enum LineReason
		{
			TRACE = 0,	// flag encapsulates function log
			INFO,		// flag useful information
			DEBUG,		// flag for dev only
			WARN,		// flag 
			ERROR,		// flag
			CRITICAL	// flag
		};
		Q_ENUM(LineReason);

		enum OutputMode
		{
			Write,		// flag enables writing to file
			Print,		// flag enables printing to console

			PrintWrite = Print | Write // flag enables all
		};
		Q_DECLARE_FLAGS(OutputModeFlags, OutputMode);

		enum LineFormatting
		{
			Date,		// flag appends current date at the beginning of the line
			Object,		// flag appends object name which sent log
			Reason,		// flag appends line information

			All	= Reason | Object | Date // flag includes all format
		};
		Q_DECLARE_FLAGS(LineFormattingFlags, LineFormatting);

		struct Line
		{
			QDateTime			DateTime;
			QObject*			Sender;
			Logging::LineReason	Type;
			QString				Info;

			friend QDebug& operator<<(QDebug& os, const Line& line);
			friend std::ostream& operator<<(std::ostream& os, const Line& line);
		};
		/**
		 *	@param logPath - where the log file will be stored.
		 * 	@param flags - logging information.
		 *	@return inited state
		 *
		 *	DO: Initializes class and opens thread to write data in file
		 */
		static bool Start(Logging::LineFormattingFlags flags = Logging::LineFormatting::All, Logging::OutputMode mode = Logging::OutputMode::PrintWrite);
		/**
		 *	DO: Stops logging thread
		 */
		static void Stop();

	public slots:
		/**
		 *	@param info - logging information
		 *	@param type - information type
		 *	@param sender - object who logged
		 *
		 *	DO: 
		 */
		static void trace(QObject* sender, QStringView info);
		/**
		 *	@param info - logging information
		 *	@param type - information type
		 *	@param sender - object who logged
		 *
		 *	DO: 
		 */
		static void debug(QObject* sender, QStringView info);
		/**
		 *	@param info - logging information
		 *	@param sender - object who logged
		 *
		 *	DO: 
		 */
		static void info(QObject* sender, QStringView info);
		/**
		 *	@param info - logging information
		 *	@param sender - object who logged
		 *
		 *	DO: 
		 */
		static void warn(QObject* sender, QStringView info);
		/**
		 *	@param info - logging information
		 *	@param sender - object who logged
		 *
		 *	DO: 
		 */
		static void error(QObject* sender, QStringView info);
		/**
		 *	@param info - logging information
		 *	@param sender - object who logged
		 *
		 *	DO:
		 */
		static void critical(QObject* sender, QStringView info);

	private:
		/**
		 *	@param format -
		 *
		 *	DO:
		 */
		void SetLineFlags(Logging::LineFormattingFlags format);
		/**
		 *	@param mode -
		 *
		 *	DO: 
		 */
		void SetOutputFlags(Logging::OutputModeFlags mode);
		/**
		 *	@param  -
		 *
		 *	DO:
		 */
		void SetDateTimeFormat(QStringView format);
		/**
		 *	@param  -
		 *
		 *	DO: 
		 */
		void SetLoggerPattern(QStringView pattern);
		/**
		 *	@param  -
		 *
		 *	DO:
		 */
		void SetLogFilePath(const std::filesystem::path& path);

	private:
		static Logging* Get();
		/**
		 *	@param info - logging information
		 *	@param type - information type
		 *	@param sender - object who logged
		 *
		 *	DO: writes data to new line in opened file log
		 */
		static void write(QStringView info, LineReason type, QObject* sender = nullptr);
		/**
		 *	@param info - logging information
		 *	@param type - information type
		 *	@param sender - object who logged
		 *
		 *	DO:
		 *	LINE TYPE: ALL
		 */
		static void log(QStringView info, LineReason type, QObject* sender = nullptr);

	signals:
		void OnLogEvent();

	public:
		explicit Logging(QObject* parent = nullptr);
		Logging(const Logging&) = delete;
		void operator=(const Logging&) = delete;
		~Logging() override;

	private:
		static Logging* s_instance;

		// Logger states
		bool isStarted, isDate, isObject, isReason,
			toFile, toConsole;

		QString m_timeFormat, m_logPattern;

		QFile* File;

        std::vector <Line> m_queuedLines;

		QThread* m_logThread;
		mutable QMutex m_mutex;
	};

	Q_DECLARE_OPERATORS_FOR_FLAGS(Logging::OutputModeFlags);
	Q_DECLARE_OPERATORS_FOR_FLAGS(Logging::LineFormattingFlags);
}

#ifdef _DEBUG
	#define ALO_DEBUG(...) \
		::alo::Utils::Logging::debug(__VA_ARGS__)
#else
	#define ALO_DEBUG(...)
#endif
	
#define ALO_INFO(...) \
	std::cout << __FUNCDNAME__
	//::alo::Utils::Logging::info(__VA_ARGS__)

#define ALO_WARN(...) \
	::alo::Utils::Logging::warn(__VA_ARGS__)

#define ALO_ERROR(...) \
	::alo::Utils::Logging::error(__VA_ARGS__)

#define ALO_CRITICAL(...) \
	::alo::Utils::Logging::critical(__VA_ARGS__)