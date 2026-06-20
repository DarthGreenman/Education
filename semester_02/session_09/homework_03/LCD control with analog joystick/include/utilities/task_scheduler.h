// task_scheduler.h

#ifndef TASK_SHEDULER_H_FOR_THE_ARDUINO_UNO_PROJECT
#define TASK_SHEDULER_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <stdint.h>

namespace uno {

	/**********************************************************************************************
	 * @brief Класс для управления задачами, который позволяет планировать выполнение функций-членов
	 * объектов с определённым интервалом времени.
	 *
	 * Класс использует шаблоны для обеспечения гибкости и может работать с любыми типами объектов
	 * и функциями-членами, которые соответствуют заданной сигнатуре.
	 * @tparam T       тип объекта, для которого создается задача.
	 * @tparam ...Args типы аргументов функции-члена объекта.
	 */
	template <typename T, typename... Args>
	class task_scheduler {
	public:
		using value_type = T;
		using const_pointer_to_function_type = void (T::* const)(Args...);

		constexpr task_scheduler() = default;

		/******************************************************************************************
		 * @brief Конструктор для создания задачи с функцией-членом объекта и ссылкой на объект.
		 *
		 * @param function              указатель на функцию-член объекта, которая будет выполняться
		 * @param object                ссылка на объект, для которого создается задача
		 * @param polling_time_interval интервал опроса в миллисекундах
		 */
		explicit task_scheduler(const_pointer_to_function_type function, value_type& object,
			uint32_t polling_time_interval) :
			_object{ object }, _function{ function }, _time_params{ polling_time_interval, millis() } {}

		task_scheduler(const task_scheduler&) = delete;
		task_scheduler(task_scheduler&&) = delete;
		~task_scheduler() = default;

		task_scheduler& operator=(const task_scheduler&) = delete;
		task_scheduler& operator=(task_scheduler&&) = delete;

		/******************************************************************************************
		 * @brief Выполнить задачу, если пришло время для её выполнения.
		 *
		 * Метод проверяет наступил ли момент запуска задачи, если да, то вызывается метод объекта с
		 * переданными аргументами.
		 * @param run     флаг, указывающий, нужно ли выполнять задачу
		 * @param ...args аргументы, передаваемые функции-члену объекта
		 */
		/* FORCEINLINE */ void exec(bool run, Args... args) {
			if(!run)
				return;

			const auto curr_time = millis();
			if(curr_time - _time_params.second >= _time_params.first) {
				_time_params.second = curr_time;
				(_object.*_function)(args...);
			}
		}

	private:
		value_type& _object{};
		const_pointer_to_function_type _function{};
		/* @note Структура для хранения информации о времени выполнения задачи, включая интервал опроса,
		 * время последнего выполнения */
		utl::pair<uint32_t, uint32_t>  _time_params{};
	};

} /// !namespace uno

#endif /// !TASK_SHEDULER_H_FOR_THE_ARDUINO_UNO_PROJECT

