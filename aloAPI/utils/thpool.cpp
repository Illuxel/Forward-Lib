#include "thpool.h"

namespace alo::Utils
{
	void ThreadPool::Init()
	{
		// m_threadPool.resize(std::thread::hardware_concurrency());
	}

    const int& ThreadPool::GetThreadAmount() const
	{
		return 0;
	}
}
