#include "log.h"
#include "converters.h"


namespace alo::Utils {

	Logging* Logging::s_instance{ nullptr };
	
	Logging* Logging::Get()
	{
		if (!s_instance) s_instance = new Logging();
		return s_instance;
	}

	bool Logging::Start(Logging::LineFormattingFlags flags, Logging::OutputMode mode)
	{
		Logging* log = Get();

		log->SetLineFlags(flags);

		if (!log->toFile)
			return true;

		log->File = new QFile("log_" + CurrentDateTimeStr() + ".log");

		if (log->File->open(QFile::WriteOnly | QFile::Text))
		{
			
            //ALO_INFO("Logging successfully started.", log);
			return true;
		}
		else
		{
            ALO_ERROR(log, log->File->errorString());
			return false;
		}
	}
	void Logging::Stop()
	{
		Logging* log = Get();
		log->isStarted = false;
		log->File->close();
		delete log->File;
	}

	void Logging::trace(QObject* sender, QStringView info)
	{
		
	}
	void Logging::debug(QObject* sender, QStringView info)
	{

	}
	void Logging::info(QObject* sender, QStringView info)
	{
		
	}
	void Logging::warn(QObject* sender, QStringView info)
	{
		
	}
	void Logging::error(QObject* sender, QStringView info)
	{
		
	}
	void Logging::critical(QObject* sender, QStringView info)
	{
	
	}

	void Logging::log(QStringView info, LineReason type, QObject* sender)
	{
	}

	void Logging::write(QStringView info, LineReason type, QObject* sender)
	{
		Logging* log = Get();

		if (!log->File)
			return;
		if (!log->File->isOpen())
			return;
		if (!log->toFile)
			return;

		QTextStream stream(log->File);
		stream << info.toUtf8() + '\n';
	}

	void Logging::SetDateTimeFormat(QStringView timeFormat)
	{
		m_timeFormat.clear();
		m_timeFormat.insert(0, timeFormat);
	}
	void Logging::SetLoggerPattern(QStringView pattern)
	{
		m_timeFormat.clear();
		m_logPattern.insert(0, pattern);
	}

	void Logging::SetLogFilePath(const std::filesystem::path& path)
	{

	}

	void Logging::SetLineFlags(Logging::LineFormattingFlags flags)
	{
		if (flags.testFlag(LineFormatting::Date))
			isDate = true;
		if (flags.testFlag(LineFormatting::Object))
			isObject = true;
		if (flags.testFlag(LineFormatting::Reason))
			isReason = true;
	}
	void Logging::SetOutputFlags(Logging::OutputModeFlags mode)
	{
		if (mode.testFlag(OutputMode::Print))
			toConsole = true;
		if (mode.testFlag(OutputMode::Write))
			toFile = true;
	}

	Logging::Logging(QObject* parent)
		: QObject(parent)
		, File(nullptr), m_logThread(nullptr)
		, isStarted(false), isDate(false), isObject(false), isReason(false)
		, toConsole(false), toFile(false)
	{
		setObjectName("Logger");
	}
	Logging::~Logging() {}

	
	QDebug& operator<<(QDebug& os, const Logging::Line& line)
	{
		// TODO: insert return statement here

		return os;
	}
	std::ostream& operator<<(std::ostream& os, const Logging::Line& line)
	{
		// TODO: insert return statement here

		return os;
	}
}