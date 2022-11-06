#pragma once

#include "aloAPI/AloAPI_global.h"

#include <thread>
#include <mutex>

#include <queue>

namespace alo::Utils
{
	class ALO_API ThreadPool :
		public QObject
	{
		explicit ThreadPool() = default;

	public:
		static void Init();

		const int& GetThreadAmount() const;

		ThreadPool(const ThreadPool&) = delete;
		void operator=(const ThreadPool&) = delete;
		~ThreadPool() = default;

	private:
		static std::mutex m_lock;

		std::vector <std::thread> m_Pool;
        size_t m_ThreadAmount;
	};
}
