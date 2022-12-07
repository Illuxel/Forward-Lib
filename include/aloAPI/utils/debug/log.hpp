#pragma once

#include "aloAPI/alo-common.hpp"

#include "aloAPI/utils/debug/logger.hpp"

namespace alo::Utils {

	class ALO_API Log
	{
		explicit Log();

	public:
		static Log* Get();
		/**
		 *
		 *	DO: 
		 */
		static bool Start();
		/**
		 *	DO: 
		 */
		static void Stop();

		~Log();

	private:
		static Log* s_instance;

		bool isStarted;
	};
}

#ifdef _DEBUG
#define ALO_DEBUG (...) \
	// std::cout << ""
// #else
// #define ALO_DEBUG (...)
#endif

#define ALO_TRACE (...) \

#define ALO_INFO (...) \

#define ALO_WARN (...) \

#define ALO_ERROR (...) \

#define ALO_CRITICAL (...) \