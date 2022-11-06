#pragma once

#include "aloAPI/AloAPI_global.h"

#include <QDateTime>
#include <QFile>

namespace alo::Utils {

    struct ALO_API LogString;

    class ALO_API Log : QObject
	{
		Q_OBJECT

		explicit Log(QObject* parent = nullptr);

	public:
		Log(const Log& right) = delete;
		void operator=(const Log&) = delete;
		~Log() override;

		enum InfoType
		{
			INFO = 0,
			ERROR,
			WARN
		};
		Q_ENUM(InfoType);

		enum IncludeLineFormat
		{
			Date = 3,	// appends current date at the beginning of the line
			Object,		// appends object name which sent log
			LineType,	// appends line information
			Trace,		// 

			All	= LineType | Object | Date // includes all format
		};
		Q_DECLARE_FLAGS(LogLineFormat, IncludeLineFormat)
		/**
		 *	@param logPath - where the log file will be stored.
		 * 	@param flags - logging information.
		 *	@return inited state
		 *	
		 *	TO DO: Initializes class and opens thread to write data in file
		 */ 
		static bool Init(const QString& logPath = "", Log::LogLineFormat flags = Log::All);
		/**
		 *	TO DO: Stops logging
		 */
		static void Stop();

	public slots:
		/**
		 *	@param info - logging information
		 *	@param sender - object who logged
		 *
		 *	TO DO: 
		 *  TYPE: INFO
		 */
		static void info(const QString& info, QObject* sender = nullptr);
		/**
		 *	@param info - logging information
		 *	@param sender - object who logged
		 *
		 *	TO DO: 
		 *	TYPE: WARNING
		 */
		static void warn(const QString& info, QObject* sender = nullptr);
		/**
		 *	@param info - logging information
		 *	@param sender - object who logged
		 *
		 *	TO DO: 
		 *	TYPE: ERROR
		 */
		static void error(const QString& info, QObject* sender = nullptr);
		/**
		 *	TO DO: Returns time format
		 */
		static const QString& GetTimeFormat();

	/*signals:
		void OnLogEvent(const alo::Utils::LogString& log);*/

	private:
		static Log* Get();
		/**
		 *	@param info - logging information
		 *	@param type - information type
		 *	@param sender - object who logged
		 *
		 *	TO DO:
		 *	LINE TYPE: ALL 
		 */
		static void debug(const QString& info, InfoType type, QObject* sender);
		/**
		 *	@param info - logging information
		 *
		 *	TO DO: writes data to current opened log file
		 */
		static void WriteToFile(const QString& info);
		/**
		 *	@param str - 
		 * 
		 *	TO DO:
		 */
		QString appendLogFormat(const LogString& str) const;
		/**
		 *	@param flags - 
		 * 
		 *	TO DO: 
		 */
		void setLineFormat(Log::LogLineFormat flags);
		/**
		 *	@param flags -
		 *
		 *	TO DO: Creates an object with type LogString 
		 *		   and sets date, caller, type
		 */
		LogString createLogString(InfoType type, QObject* sender = nullptr) const;

	private:
		static Log* s_instance;

		// time format of logging
		const QString& TimeFormat = "dd/MM/yy hh:mm:ss";
		
		// Logger states
		bool isStarted, isDate, isObject, isLineType, isWriteable;

		// 
        std::vector <LogString> QueueLines;

		// allows to create and write data in file with specified format
		QFile* File = nullptr;
	};

    struct ALO_API LogString
	{
		QDateTime		DateTime;
		QObject*		Sender;
		Log::InfoType	Type;
		QString			Info;

        friend QDebug& operator<<(QDebug os, const LogString& line)
        {
            os << QString("%1 [%2][%3] %4").
                arg(line.DateTime.toString(Log::GetTimeFormat())).
                arg((line.Sender == nullptr)
                    ? "" : "Logger").
                arg(QVariant::fromValue(line.Type).toString()).
                arg(line.Info);

            return os;
        }
        friend std::ostream& operator<<(std::ostream& os, const LogString& line)
        {
            os << QString("%1 [%2][%3] %4").
                arg(line.DateTime.toString(Log::GetTimeFormat())).
                arg((line.Sender == nullptr)
                    ? "" : "Logger").
                arg(QVariant::fromValue(line.Type).toString()).
                arg(line.Info).
                toStdString();

            return os;
        }
	};

	Q_DECLARE_OPERATORS_FOR_FLAGS(Log::LogLineFormat);
}

#ifdef _DEBUG
#define MSG_INFO(inf, obj) \
	::alo::Utils::Log::info(inf, obj)

#define MSG_WARN(inf, obj) \
	::alo::Utils::Log::warn(inf, obj)

#define MSG_ERROR(inf, obj) \
	::alo::Utils::Log::error(inf, obj)

#define MSG_EXCEPTION(inf, obj, exc) \
	::alo::Utils::Log::error(inf, obj); \
    throw exc
#else
#define MSG_INFO(inf, obj)
#define MSG_WARN(inf, obj)
#define MSG_ERROR(inf, obj)
#define MSG_EXCEPTION(inf, obj, exc)
#endif
