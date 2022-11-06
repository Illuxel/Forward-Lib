#include "log.h"
#include "converters.h"

#include <filesystem>

#include <QDebug>
#include <QVariant>

namespace alo::Utils
{
	Log::Log(QObject* parent)
        : QObject(parent), isStarted(false)
        , isDate(false), isObject(false)
        , isLineType(false), isWriteable(false)
	{
		setObjectName("Logger");
	}
	Log::~Log()
	{
		Stop();
	}
    Log* Log::s_instance{ nullptr };
	Log* Log::Get()
	{
		if (!s_instance) s_instance = new Log();
		return s_instance;
	}

	bool Log::Init(const QString& logPath, Log::LogLineFormat flags)
	{
		Log* log = Get();
		log->isStarted = true;

		log->setLineFormat(flags);

		if (!log->isWriteable)
			return true;

		QString filePath(logPath + "log_" + CurrentDateTimeStr("yyMMddhhmmss") + ".log");
		log->File = new QFile(filePath);

		if (log->File->open(QFile::WriteOnly | QFile::Text))
		{
            MSG_INFO("Logging successfully started.", log);
			return true;
		}
		else
		{
            MSG_WARN(log->File->errorString(), log);
			return false;
		}
	}
	void Log::Stop()
	{
		Log* log = Get();
		log->File->close();
		delete log->File;
	}

    void Log::debug(const QString& info, InfoType type, QObject* sender)
	{
		LogString line = Log::Get()->createLogString(type, sender);
		line.Info = info;

		const QString& str = Log::Get()->appendLogFormat(line);

		qDebug() << str;
		Log::WriteToFile(str);

		//emit Log::Get()->OnLogEvent(line);
	}

	void Log::info(const QString& info, QObject* sender)
	{
		debug(info, INFO, sender);
	}
	void Log::warn(const QString& info, QObject* sender)
	{
		debug(info, WARN, sender);
	}
	void Log::error(const QString& info, QObject* sender)
	{
		debug(info, ERROR, sender);
	}

	const QString& Log::GetTimeFormat()
	{
		return Log::Get()->TimeFormat;
	}

	void Log::WriteToFile(const QString& info)
	{
		if (!Log::Get()->isWriteable)
			return;

		if (Log::Get()->File->isOpen())
			return;

		QTextStream stream(Log::Get()->File);
		stream << info.toUtf8() + '\n';
	}

	QString Log::appendLogFormat(const LogString& line) const
	{
		QString lineData = "";

		if (isDate) 
		{
			lineData.append(line.DateTime.toString(TimeFormat) + ' ');
		}
		if (isObject)
		{
			if (!line.Sender)
			{
				lineData.append('[' + Log::Get()->objectName() + ']');
			}
			else 
			{
				if (line.Sender->objectName().isEmpty())
					lineData.append("Logger");
				else
					lineData.append(('[' + line.Sender->objectName() + ']'));
			}
		}
		if (isLineType)
		{
			lineData.append('[' + EnumToStr(line.Type) + "] ");
		}

		lineData.append(line.Info);

		return lineData;
	}

	void Log::setLineFormat(Log::LogLineFormat flags)
	{
		if (flags.testFlag(IncludeLineFormat::Date))
			isDate = true;
		if (flags.testFlag(IncludeLineFormat::Object))
			isObject = true;
		if (flags.testFlag(IncludeLineFormat::LineType))
			isLineType = true;
	}

	LogString Log::createLogString(InfoType type, QObject* sender) const
	{
		LogString line;

		line.DateTime = QDateTime::currentDateTime();
		line.Sender = sender;
		line.Type = type;

		return line;
	}


}
