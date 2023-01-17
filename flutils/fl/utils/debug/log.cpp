#include "fl/utils/debug/log.hpp"
#include "fl/utils/debug/logger.hpp"
#include "fl/utils/converters.hpp"

namespace fl::utils {

	Log::Log() {}
	Log::~Log() {}

	std::unordered_map<std::string, Logger*> Log::s_logPool{};

	Logger* Log::Get(const std::string& name)
	{
		return s_logPool.at(name);
	}

	void Log::Innit(const std::string& name)
	{
		Logger* logger = new Logger();
		s_logPool.emplace(std::make_pair(name, logger));
	}
	void Log::Remove(const std::string& name)
	{
		
	}
}