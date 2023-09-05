#pragma once

#include "fl/db/Core.hpp"

namespace Forward::MySQL {

	class DriverLock
	{
	private:
		sql::Driver* driver_;

	public:
		DriverLock(sql::Driver* driver);
		~DriverLock();

		void Lock();
		void Unlock();
	};
}