#pragma once

#include <jdbc/mysql_driver.h>

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