// task.h

#ifndef TASK_H_FOR_THE_ARDUINO_UNO_PROJECT
#define TASK_H_FOR_THE_ARDUINO_UNO_PROJECT

#ifdef _ARDUINO_STUBS
#include "arduino_stubs.h"
#else
#include <Arduino.h>
#endif
#include <stdint.h>

namespace uno {
	template <typename T, typename... Args>
	class task {
	public:
		using const_pointer_to_function_type = void (T::* const)(Args...);
		using const_pointer_to_object_type = T* const;

		task() = default;
		/**
		 * @brief Construct a new task object
		 *
		 * @param function
		 * @param object
		 * @param polling_time_interval
		 * @param time_before_start
		 */
		explicit task(const_pointer_to_function_type function, const_pointer_to_object_type object,
			uint32_t polling_time_interval, uint32_t time_before_start = 0ul)
			: _object{ T{} }, _cptr_object{ object }, _cptr_func{ function }
			, _timeslot{ polling_time_interval, millis() + time_before_start, millis() }
			, _we_work{} {}

		/**
		 * @brief Construct a new task object
		 *
		 * @param function
		 * @param object
		 * @param polling_time_interval
		 * @param time_before_start
		 */
		explicit task(const_pointer_to_function_type function, T&& object,
			uint32_t polling_time_interval, uint32_t time_before_start = 0ul)
			: _object{ object }, _cptr_object{ &_object }, _cptr_func{ function }
			, _timeslot{ polling_time_interval, millis() + time_before_start, millis() }
			, _we_work{} {}

		task(const task&) = default;
		task(task&&) = default;
		~task() = default;

		task& operator=(const task&) = default;
		task& operator=(task&&) = default;

		/**
		 * @brief
		 *
		 */
		void exec(bool run, Args... args) {
			if(!run)
				return;

			const auto curr_time = millis();
			if(!_we_work)
				_we_work = wework(curr_time);

			if(_we_work && curr_time - _timeslot.work >= _timeslot.polling_interval) {
				_timeslot.work = curr_time;
				(_cptr_object->*_cptr_func)(args...);
			}
		}

	private:
		bool wework(uint32_t curr_time) const { return _timeslot.to_work == curr_time ? true : false; }

		T _object{};
		const_pointer_to_object_type _cptr_object{};
		const_pointer_to_function_type _cptr_func{};

		/**
		 * @brief 
		 * 
		 */
		struct timeslot {
			uint32_t polling_interval;
			uint32_t to_work;
			uint32_t work;
		} _timeslot{};

		bool _we_work{};
	};
} // namespace uno

#endif // !TASK_H_FOR_THE_ARDUINO_UNO_PROJECT
