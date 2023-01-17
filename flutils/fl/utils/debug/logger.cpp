#include "fl/utils/debug/logger.hpp"
#include "fl/utils/StringBuilder.hpp"
#include "fl/utils/converters.hpp"

#include <QFile>
#include <QDateTime>

namespace fl::utils {
	
	Logger::Logger(QObject* parent)
		: QObject(parent)
	{
		setObjectName("Logger");

		SetOutputMode(OutputMode::PrintWrite);

		SetDateTimeFormat("");

		SetMessagePattern("");
		SetFileNamePattern("");

		CreateFile();
	}
	Logger::Logger(std::string_view fileName, QObject* parent)
		: QObject(parent)
	{
		CreateFile();
	}
    Logger::~Logger()
    {
		m_LogFile.close();
    }

	void Logger::CreateFile()
	{
		if (m_LogFile.is_open())
			return;

		m_LogFile.open();
	}
	
	void Logger::SetDateTimeFormat(std::string_view timeFormat)
	{
		m_TimeFormat = timeFormat;
	}
	void Logger::SetFileNamePattern(std::string_view filePattern)
	{
		m_FileNamePattern = filePattern;
		ParseFileNameFormat();
	}
	void Logger::SetMessagePattern(std::string_view msgPattern)
	{
		m_LogMessagePattern = msgPattern;
		ParseMessageFormat();
	}

	void Logger::SetOutputMode(Logger::OutputModeFlags mode)
	{
		if (mode.testFlag(OutputMode::Print))
			toConsole = true;
		if (mode.testFlag(OutputMode::Write))
			toFile = true;
	}

	std::string_view Logger::ParseFileNameFormat() const
	{
		return m_ParsedFileName;
	}
	std::string_view Logger::ParseMessageFormat() const
	{
		return m_ParsedMessageFormat;
	}

	Logger::Message& Logger::log(Logger::Level level, QObject* object) {
		Message line;

		line.Level = level;
		line.Sender = object;
		line.DateTime = QDateTime::currentDateTime();

		m_LogMessages.push(line);

		return m_LogMessages.front();
	}

	Logger::Message& Logger::Message::operator<<(const char* msg) 
	{
		this->Info = msg;
		return *this;
	}
    QDebug operator<<(QDebug debug, const Logger::Message &line)
    {
		QDebugStateSaver saver(debug);
		debug.nospace() << line.Level << line.Sender << line.DateTime << QString::fromStdString(line.Info) << '\n';
		return debug;
	}
}