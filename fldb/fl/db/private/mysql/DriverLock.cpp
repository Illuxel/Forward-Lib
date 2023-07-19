#include "DriverLock.hpp"
#include "fl/db/private/mysql/DriverLock.hpp"

namespace Forward::MySQL {

	DriverLock::DriverLock(sql::Driver* driver)
		: driver_(driver)
	{
		Lock();
	}
	DriverLock::~DriverLock()
	{
		Unlock();
	}
	void DriverLock::Lock()
	{
		driver_->threadInit();
	}
	void DriverLock::Unlock()
	{
		driver_->threadEnd();
	}
}
