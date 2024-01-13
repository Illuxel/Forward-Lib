#pragma once

#include <chrono>

namespace Forward {

	class Timer 
	{
	private:
		std::chrono::steady_clock::time_point time_start_, time_end_;

	public:
		Timer()
		{
			Start();
		}

		void Start()
		{
			time_start_ = std::chrono::high_resolution_clock::now();
		}
		void Stop() 
		{
			time_end_ = std::chrono::high_resolution_clock::now();
		}

		std::chrono::microseconds Result() const
		{
			return std::chrono::duration_cast<std::chrono::microseconds>(time_end_ - time_start_);
		}
	};
}