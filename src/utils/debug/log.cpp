#include "aloAPI/utils/debug/log.hpp"
#include "aloAPI/utils/converters.hpp"

namespace alo::Utils {

	Log::Log() {}
	Log::~Log() {}

	Log* Log::s_instance{ nullptr };
	Log* Log::Get()
	{
		if (!s_instance) s_instance = new Log();
		return s_instance;
	}

	bool Log::Start()
	{
		return true;
	}
	void Log::Stop()
	{

	}
}