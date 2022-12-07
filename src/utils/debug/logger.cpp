#include "aloAPI/utils/debug/logger.hpp"

#include <QFile>

namespace alo::Utils {
	Logger::Logger(QObject* parent)
		: QObject(parent){}

	void Logger::SetFilePath(const std::filesystem::path& path)
	{
		
	}
	void Logger::SetFileName(QStringView fileName)
	{
		m_LogFile->setFileName(fileName.toString());
	}
	void Logger::SetDateTimeFormat(QStringView timeFormat)
	{
		m_TimeFormat.clear();
		m_TimeFormat.insert(0, timeFormat);
	}
	void Logger::SetFileNamePattern(QStringView filePattern)
	{
		m_FileNamePattern.clear();
		m_FileNamePattern.insert(0, filePattern);
	}
	void Logger::SetMessagePattern(QStringView msgPattern)
	{
		m_LogMessagePattern.clear();
		m_LogMessagePattern.insert(0, msgPattern);
	}
	void Logger::SetOutputMode(Logger::OutputModeFlags mode)
	{
		if (mode.testFlag(OutputMode::Print))
			toConsole = true;
		if (mode.testFlag(OutputMode::Write))
			toFile = true;
	}

	void Logger::trace() {}

	void Logger::info()
	{
	}
	void Logger::debug()
	{
	}
	void Logger::warn()
	{
	}
	void Logger::error()
	{
	}
	void Logger::critical()
	{
	}

 	QDebug& operator<<(QDebug& os, const Logger::LogMessage& line)
	{
		// TODO: insert return statement here

		return os;
	}
	std::ostream& operator<<(std::ostream& os, const Logger::LogMessage& line)
	{
		// TODO: insert return statement here

		return os;
	}
}