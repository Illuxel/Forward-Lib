#include "fl/utils/Log.hpp"

namespace Forward {

	Log::Log() {}
	Log::~Log() {}

	Log& Log::Instance() 
	{
		static Log log;
		return log;
	}

	Ref<Logger> Log::Get(std::string_view log_name)
	{
		Log& log = Log::Instance();

		std::shared_lock lock(log.pool_mtx_);

		auto const& it = log.pool_.find(log_name.data());

		if (it == log.pool_.end())
			return nullptr;

		return log.pool_.at(log_name.data());
	}

	void Log::Init(std::string_view log_name)
	{
		Log& log = Log::Instance();

		std::unique_lock lock(log.pool_mtx_);

		Ref<Logger> logger = MakeRef<Logger>();
		log.pool_.emplace(log_name, logger);
	}

	void Log::Remove(std::string_view log_name)
	{
		Log& log = Log::Instance();
		Ref<Logger> logger = Log::Get(log_name);

		if (!logger)
			return;

		{
			std::shared_lock lock(log.pool_mtx_);
			log.pool_.erase(log_name.data());
		}

		logger.reset();
	}
}
