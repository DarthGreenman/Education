// safe_queue.h

#ifndef SAFE_QUEUE_H_IN_MY_PROJECT
#define SAFE_QUEUE_H_IN_MY_PROJECT

#include <memory>
#include <mutex>
#include <queue>

// Не передавайте указатели и ссылки на защищенные данные 
// за пределы области видимости блокировки никаким способом, будь то:
// 1. Возврат из функции.
// 2. Сохранение в видимой извне памяти.
// 3. Передача в виде аргумента пользовательской функции.

// Третий вариант – возвращать не копию вытолкнутого элемента по значению, а указатель на него.
// Его достоинство в том, указатели можно копировать, не опасаясь исключений. А недостаток в том,
// что возврат указателя заставляет искать средства для управления выделенной объекту памятью,
// так что для таких простых типов, как целые числа, накладные расходы на управление памятью
// могут превысить затраты на возврат типа по значению. 
// В любом интерфейсе, где применяется этот вариант, в качестве типа указателя разумно избрать
// std::shared_ptr. Мало того что это предотвращает утечки памяти, поскольку объект уничтожается
// вместе с уничтожением последнего указателя на него, так еще и библиотека полностью контролирует
// схему распределения памяти и не требует использования new и delete.

namespace multitask
{
	template<typename T>
	class safe_queue
	{
	public:
		using value_type = T;
		using unique_pointer = typename std::unique_ptr<T>;

		safe_queue() = default;

		~safe_queue() = default;

		void push(const value_type& value)
		{
			std::lock_guard<std::mutex> lk{ _mutex };
			_queue.push(value);
			_wake.notify_all();
		}
		void push(value_type&& value)
		{
			std::lock_guard<std::mutex> lk{ _mutex };
			_queue.push(std::move(value));
			_wake.notify_all();
		}
		decltype(auto) front()
		{
			std::lock_guard<std::mutex> lk{ _mutex };
			if (_queue.empty())
				return std::unique_ptr<value_type>{};
			auto value = std::make_unique<value_type>(std::move(_queue.front()));
			_queue.pop();
			return value;
		}
		decltype(auto) empty() const
		{
			std::lock_guard<std::mutex> lk(_mutex);
			return _queue.empty();
		}

	private:
		mutable std::mutex _mutex{}; // copy constructor is deleted, unsigned __int64
		std::condition_variable _wake{}; // copy constructor is deleted, unsigned __int64
		std::queue<value_type> _queue{};

	public:
		safe_queue(const safe_queue&) = delete;
		safe_queue(safe_queue&&) = delete;
		safe_queue& operator=(const safe_queue&) = delete;
		safe_queue& operator=(safe_queue&&) = delete;

	}; // class safe_queue
} // namespace multitask

#endif // !SAFE_QUEUE_H_IN_MY_PROJECT

