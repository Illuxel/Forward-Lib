#include "fl/utils/Log.hpp"

namespace fl::utils {

	std::unordered_map<std::string, Ref<Logger>> Log::logs_{};

	Ref<Logger> Log::Get(std::string_view name)
	{
		auto const& it = logs_.find(name.data());

		if (it != logs_.end())
			return logs_.at(name.data()); 

		Log::Innit(name);
		return Log::Get(name);
	}

	void Log::Innit(std::string_view log_name)
	{
		auto const& logger = MakeRef<Logger>();
		logs_.emplace(std::make_pair(log_name, logger));
	}

	void Log::Remove(std::string_view log_name)
	{
		auto const& it = logs_.find(log_name.data());

		if (it != logs_.end())
			return;

		auto const name = it->first;
		auto& logger = it->second;

		logs_.erase(name);
		logger.reset();
	}
}