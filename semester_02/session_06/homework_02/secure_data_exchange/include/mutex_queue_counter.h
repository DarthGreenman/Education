// mutex_queue_counter.h

#ifndef MUTEX_QUEUE_COUNTER_H
#define MUTEX_QUEUE_COUNTER_H

#include <cstddef>
#include <iosfwd>
#include <limits>
#include <mutex>
#include <utility>

namespace th
{
	class mutex_queue_counter
	{
	public:
		friend auto operator<(const mutex_queue_counter&, std::size_t);
		friend auto operator==(const mutex_queue_counter&, std::size_t);
		friend auto swap(mutex_queue_counter&, mutex_queue_counter&) noexcept;

		mutex_queue_counter() = default;
		mutex_queue_counter(const mutex_queue_counter&) = delete;
		mutex_queue_counter(mutex_queue_counter&&) = delete;
		~mutex_queue_counter() = default;

		mutex_queue_counter& operator=(const mutex_queue_counter&) = delete;
		mutex_queue_counter& operator=(mutex_queue_counter&&) = delete;

		auto increment()
		{
			std::lock_guard<std::mutex> lock{ _mutex };
			++_value;
		}
		auto decrement()
		{
			std::lock_guard<std::mutex> lock{ _mutex };
			--_value;
		}

		auto get_value() const
		{
			std::lock_guard<std::mutex> lock{ _mutex };
			return _value;
		}

	private:
		std::size_t _value{};
		mutable std::mutex _mutex{};
	};

	auto operator<(const mutex_queue_counter& lhs, std::size_t rhs)  { return lhs._value < rhs; }
	auto operator==(const mutex_queue_counter& lhs, std::size_t rhs) { return lhs._value == rhs; }
	auto operator!=(const mutex_queue_counter& lhs, std::size_t rhs) { return !(lhs == rhs); }

	auto swap(mutex_queue_counter& lhs, mutex_queue_counter& rhs) noexcept
	{
		if (&lhs == &rhs)
			return;
		
		/* std::lock and std::lock_guard */
		/*
		std::lock(lhs._mutex, rhs._mutex);
		std::lock_guard<std::mutex> lgl(lhs._mutex, std::adopt_lock);
		std::lock_guard<std::mutex> lgr(rhs._mutex, std::adopt_lock);
		*/

		/* std::unique_lock and std::lock */
		/*
		std::unique_lock<std::mutex> ull{ lhs._mutex, std::defer_lock };
		std::unique_lock<std::mutex> ulr{ rhs._mutex, std::defer_lock };
		std::lock(ull, ulr);
		*/

		std::scoped_lock(lhs._mutex, rhs._mutex);
		std::swap(lhs._value, rhs._value);
	}

} // namespace th
#endif // !MUTEX_QUEUE_COUNTER_H