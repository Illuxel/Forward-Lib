#pragma once

#include "aloAPI/alo-common.hpp"

class QFile;
class QDateTime;

namespace alo::Utils {
	
	class ALO_API Logger 
		: public QObject 
	{
		Q_OBJECT

	public:
		explicit Logger(QObject* parent = nullptr);

		enum Reason
		{
			TRACE = 0,	// flag encapsulates function log
			INFO,		// flag useful information
			DEBUG,		// flag for dev only
			WARN,		// flag not affected to a program
			ERROR,		// flag
			CRITICAL	// flag
		};
		enum OutputMode
		{
			Write,		// flag enables writing to file
			Print,		// flag enables printing to console

			PrintWrite = Print | Write // flag enables all
		};

		Q_ENUM(Reason);
		Q_DECLARE_FLAGS(OutputModeFlags, OutputMode);

		struct LogMessage
		{
			Logger::Reason	Type;

			QDateTime*		DateTime;
			QObject*		Sender;
			QString*		Info;

			friend QDebug& operator<<(QDebug& os, const LogMessage& msg);
			friend std::ostream& operator<<(std::ostream& os, const LogMessage& msg);
		};
		/**
		 *	@param
		 *
		 *	DO:
		 */
		void SetFilePath(const std::filesystem::path& filePath);
		/**
		 *	@param
		 *
		 *	DO:
		 */
		void SetFileName(QStringView fileName);
		/**
		 *	@param
		 *
		 *	DO:
		 */
		void SetOutputMode(Logger::OutputModeFlags mode);
		/**
		 *	@param
		 *
		 *	DO:
		 */
		void SetMessagePattern(QStringView msgPattern);
		/**
		 *	@param
		 *
		 *	DO:
		 */
		void SetDateTimeFormat(QStringView format);
		/**
		 *	@param
		 *
		 *	DO:
		 */
		void SetFileNamePattern(QStringView filePattern);

	public slots:
		/**
		 *	@param
		 *	@param
		 *
		 *	DO:
		 */
		void trace();
		/**
		 *	@param
		 *	@param
		 *
		 *	DO:
		 */
		void info();
		/**
		 *	@param
		 *	@param
		 *
		 *	DO:
		 */
		void debug();
		/**
		 *	@param
		 *	@param
		 *
		 *	DO:
		 */
		void warn();
		/**
		 *	@param
		 *	@param
		 *
		 *	DO:
		 */
		void error();
		/**
		 *	@param
		 *	@param
		 *
		 *	DO:
		 */
		void critical();

    private:
		/**
		 *	@param 
		 *
		 *	DO: 
		 */
		template <typename ...Args>
		static void write(Logger::Reason reason, Args&& ...args) {}
		/**
		 *	@param 
		 *
		 *	DO:
		 */
		template <typename ...Args>
		static void log(Logger::Reason reason, Args&& ...args) {}

	private:
		// Logger states
		bool isDate, isObject, isReason,
			toFile, toConsole;

		QString m_TimeFormat, 
			m_LogMessagePattern, m_FileNamePattern;

		std::vector <LogMessage> m_QueuedLines;

		QFile* m_LogFile;
	};

	Q_DECLARE_OPERATORS_FOR_FLAGS(Logger::OutputModeFlags);
}
