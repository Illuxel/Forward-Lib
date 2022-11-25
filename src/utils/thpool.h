#pragma once

#include <QVector>

#include <QThread>
#include <QMutex>

#include "aloAPI_global.h"

namespace alo::Utils
{
	class ALO_API ThreadPool :
		public QObject
	{
		ThreadPool() = default;

	public:
		static void Init();

		int GetThreadAmount() const;

		ThreadPool(const ThreadPool&) = delete;
		void operator=(const ThreadPool&) = delete;
		~ThreadPool() = default;

	private:
		mutable QMutex m_lock;

		QVector <QThread> m_Pool;
        size_t m_ThreadAmount;
	};
}
