// atomic_queue_counter.h

#ifndef ATOMIC_QUEUE_COUNTER_H
#define ATOMIC_QUEUE_COUNTER_H

#include <atomic>
#include <chrono>
#include <thread>

#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif // DEBUG


/*
memory_order_relaxed
Relaxed operation : there are no synchronization or ordering constraints imposed on other reads or writes,
only this operation's atomicity is guaranteed.
Расслабленная операция: на другие операции чтения или записи не накладываются ограничения синхронизации или упорядочения,
гарантируется только атомарность этой операции.

****************************************************************************************************************************
memory_order_consume
A load operation with this memory order performs a consume operation on the affected memory location: no reads or writes
in the current thread dependent on the value currently loaded can be reordered before this load. Writes to data-dependent
variables in other threads that release the same atomic variable are visible in the current thread. On most platforms,
this affects compiler optimizations only.
Операция загрузки с этим порядком памяти выполняет операцию потребления в затронутом месте памяти: никакие чтения или записи
в текущем потоке, зависящие от текущего загруженного значения, не могут быть переупорядочены перед этой загрузкой.
Записи в переменные, зависящие от данных, в других потоках, которые освобождают ту же атомарную переменную, видны в
текущем потоке. На большинстве платформ это влияет только на оптимизации компилятора.

****************************************************************************************************************************
memory_order_acquire
A load operation with this memory order performs the acquire operation on the affected memory location:
no reads or writes in the current thread can be reordered before this load. All writes in other threads
that release the same atomic variable are visible in the current thread.
Операция загрузки с этим порядком памяти выполняет операцию получения в затронутом месте памяти:
никакие чтения или записи в текущем потоке не могут быть переупорядочены до этой загрузки.
Все записи в других потоках, которые освобождают ту же атомарную переменную, видны в текущем потоке.

****************************************************************************************************************************
memory_order_release
A store operation with this memory order performs the release operation: no reads or writes in the current thread
can be reordered after this store. All writes in the current thread are visible in other threads that acquire
the same atomic variable and writes that carry a dependency into the atomic variable become visible in other threads
that consume the same atomic.
Операция сохранения с этим порядком памяти выполняет операцию освобождения: никакие чтения или записи в текущем потоке
не могут быть переупорядочены после этого сохранения. Все записи в текущем потоке видны в других потоках,
которые получают ту же атомарную переменную, а записи, которые несут зависимость в атомарную переменную,
становятся видимыми в других потоках, которые потребляют ту же атомарную переменную.

****************************************************************************************************************************
memory_order_acq_rel
A read-modify-write operation with this memory order is both an acquire operation and a release operation. No memory reads or
writes in the current thread can be reordered before the load, nor after the store. All writes in other threads that release
the same atomic variable are visible before the modification and the modification is visible in other threads that acquire
the same atomic variable.
Операция чтения-изменения-записи с таким порядком памяти является как операцией получения, так и операцией освобождения.
Никакие операции чтения или записи памяти в текущем потоке не могут быть переупорядочены ни до загрузки, ни после сохранения.
Все записи в других потоках, которые освобождают ту же атомарную переменную, видны до изменения, а изменение видно
в других потоках, которые получают ту же атомарную переменную.

****************************************************************************************************************************
memory_order_seq_cst
A load operation with this memory order performs an acquire operation, a store performs a release operation,
and read-modify-write performs both an acquire operation and a release operation, plus a single total order exists in which
all threads observe all modifications in the same order.
Операция загрузки с таким порядком памяти выполняет операцию получения, сохранение выполняет операцию освобождения,
а чтение-изменение-запись выполняет как операцию получения, так и операцию освобождения, плюс существует единый общий порядок,
в котором все потоки наблюдают за всеми изменениями в одном и том же порядке.
*/

namespace th
{
	template<std::size_t Headcount>
	class atomic_queue_counter
	{
		static_assert((Headcount > 0) && "The number in the queue must be greater than zero.");

	public:
		atomic_queue_counter() = default;
		explicit atomic_queue_counter(std::memory_order memory_order) :
			_memory_order{ memory_order } {}
		atomic_queue_counter(const atomic_queue_counter&) = delete;
		atomic_queue_counter(atomic_queue_counter&&) = delete;
		~atomic_queue_counter() = default;

		atomic_queue_counter& operator=(const atomic_queue_counter&) = delete;
		atomic_queue_counter& operator=(atomic_queue_counter&&) = delete;

		auto increment(std::chrono::milliseconds wait_time)
		{
			for (std::size_t i{}; i < Headcount; ++i)
			{
				std::this_thread::sleep_for(wait_time);
				_value.fetch_add(1, _memory_order); // прием документов
#ifdef DEBUG
				std::cout << _value;
#endif // DEBUG

			}
		}

		auto decrement(std::chrono::milliseconds wait_time)
		{
			for (std::size_t i{}; i < Headcount; ++i)
			{
				while (_value == 0); // ожидание посетителя
				std::this_thread::sleep_for(wait_time);
				_value.fetch_sub(1, _memory_order); // проверка и выдача документов
#ifdef DEBUG
				std::cout << _value;
#endif // DEBUG
			}
		}

	private:
		std::memory_order _memory_order{ std::memory_order::seq_cst };
		std::atomic<std::size_t> _value{};
	};

} // namespace th

#endif // !ATOMIC_QUEUE_COUNTER_H