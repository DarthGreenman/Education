// thread_pool.h

#ifndef THREAD_POOL_H_IN_MY_PROJECT
#define THREAD_POOL_H_IN_MY_PROJECT

#include "safe_queue.h"

#include <algorithm>
#include <atomic>
#include <future>
#include <memory>
#include <thread>
#include <vector>

namespace multitask
{
	namespace helper
	{
		struct release
		{
			void operator()(std::vector<std::thread>* p) const
			{
				std::for_each(std::begin(*p), std::end(*p),
					[](typename std::vector<std::thread>::value_type& v)
					{
						if (v.joinable()) v.join();
					}
				);
			}
		}; // struct release
	} // namespace helper

	template<typename R>
	class thread_pool
	{
	public:
		thread_pool() try :
			_threads{ new std::vector<std::thread> }, _tasks{}, _complete{ false }
		{
			(*_threads).reserve(std::thread::hardware_concurrency());
			for (typename decltype(_threads)::element_type::size_type count{};
				count < std::thread::hardware_concurrency();
				++count)
			{
				(*_threads).emplace_back(&thread_pool::work, this);
			}
		}
		catch (...) { _complete = true; throw; }

		~thread_pool() { _complete = true; }

		template<typename Func, typename... Args>
		std::future<R> submit(Func func, Args&&... args)
		{
			std::packaged_task<R()> task(std::bind(func, args...));
			std::future<R> future(task.get_future());
			_tasks.push(std::move(task));
			return future;
		}

	private:
		void work()
		{
			while (!_complete)
			{
				if (typename decltype(_tasks)::unique_pointer task{ _tasks.front() };
					task)
				{
					typename decltype(_tasks)::value_type exec{ std::move(*task) };
					exec();
				}
				else
					std::this_thread::yield();
			}
		}

	private:
		std::unique_ptr<std::vector<std::thread>, helper::release> _threads;
		safe_queue<std::packaged_task<R()>> _tasks;
		std::atomic<bool> _complete;

	public:
		thread_pool(const thread_pool&) = delete;
		thread_pool(thread_pool&&) = delete;
		thread_pool& operator=(const thread_pool&) = delete;
		thread_pool& operator=(thread_pool&&) = delete;

	}; // class thread_pool
} // namespace multitask

#endif // !THREAD_POOL_H_IN_MY_PROJECT
