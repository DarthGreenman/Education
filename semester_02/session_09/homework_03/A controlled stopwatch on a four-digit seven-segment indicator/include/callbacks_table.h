/// callbacks_table.h

#ifndef TABLE_OF_POINTER_TO_CALLBACK_FUNCTION_H_FOR_THE_ARDUINO_UNO_PROJECT
#define TABLE_OF_POINTER_TO_CALLBACK_FUNCTION_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "events.h"

#include <assert.h>
#include <stdint.h>

namespace uno {

	/**********************************************************************************************
	 * @brief Класс для хранения указателей на функции-обработчики событий. Он использует шаблонный
	 * параметр E для определения типа события.
	 * @tparam E Тип события, для которого будут храниться указатели на функции-обработчики.
	 */
	template <typename E>
	class table_of_callback_functions {

	public:
		static constexpr auto number_of_events = uno::event::type_traits<E>::size;
		using Value_type = typename uno::event::type_traits<E>::type;
		using pointer_to_callback_function = void (*)();

		constexpr table_of_callback_functions() = default;
		table_of_callback_functions(const table_of_callback_functions&) = default;
		table_of_callback_functions(table_of_callback_functions&&) = default;
		~table_of_callback_functions() = default;

		table_of_callback_functions& operator=(const table_of_callback_functions&) = default;
		table_of_callback_functions& operator=(table_of_callback_functions&&) = default;

		/******************************************************************************************
		 * @brief Добавить указатель на функцию-обработчик для конкретного события. Функция-обработчик
		 * должна быть валидной (не nullptr), иначе будет вызвано утверждение. Указатель на функцию
		 * будет сохранен в таблице обработчиков.
		 * @param event Событие, для которого добавляется обработчик.
		 * @param call Указатель на функцию-обработчик.
		 */
		void add(E event, pointer_to_callback_function call) {
			assert(call && "Handler should be a valid function pointer");
			_events[static_cast<Value_type>(event)] = call;
		}

		/******************************************************************************************
		 * @brief Получить указатель на функцию-обработчик для конкретного события. Указатель на 
		 * функцию будет возвращен из таблицы обработчиков на основе типа события. Если для данного
		 * события не был добавлен обработчик, будет возвращен nullptr.
		 * @param event Событие, для которого требуется получить обработчик.
		 * @return Указатель на функцию-обработчик или nullptr, если обработчик не был добавлен.
		 */
		pointer_to_callback_function call(E event) const { return _events[static_cast<Value_type>(event)]; }

	private:
		pointer_to_callback_function _events[number_of_events]{};
	};

} /// !namespace uno

#endif /// !TABLE_OF_POINTER_TO_CALLBACK_FUNCTION_H_FOR_THE_ARDUINO_UNO_PROJECT
