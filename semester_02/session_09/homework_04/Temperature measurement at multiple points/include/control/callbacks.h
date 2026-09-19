/**
 * @file
 * 
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 * 
 * @brief Менеджер событий.
 * 
 */

#ifndef TABLE_OF_POINTER_TO_CALLBACK_FUNCTION_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
#define TABLE_OF_POINTER_TO_CALLBACK_FUNCTION_H_FOR_THE_COMPONENT_LIBRARY_PROJECT

#include <addons.h>
#include <common.h>
#include <array.h>

#if defined(_MSC_VER)
#include <cassert>
#elif defined(__GNUC__) || defined(__clang__)
#include <assert.h>
#endif

namespace control {
	/**
	* @brief Менеджер событий.
	* @tparam E тип события
	*/
	template <typename E>
	class table_of_callback_functions {
	public:
		using event_type = E;
		using value_type = typename uno::enum_traits<event_type>::type; /**< тип идентификатора события */
		using pointer_to_callback_function = void (*)();                /**< тип указателя на функцию обработчик */

		/**
		 * Создает статический массив указателей обработчиков событий. Размер
		 * массива определяется на этапе компиляции, он равен количеству
		 * событий. 
		 */
		constexpr table_of_callback_functions() = default;
		table_of_callback_functions(const table_of_callback_functions&) = default;
		table_of_callback_functions(table_of_callback_functions&&) = default;
		~table_of_callback_functions() = default;

		table_of_callback_functions& operator=(const table_of_callback_functions&) = default;
		table_of_callback_functions& operator=(table_of_callback_functions&&) = default;

		/**
		 * @brief Добавляет указатель обработчика.
		 * 
		 * Функция сохраняет указатель в элементе с индексом идентификатора события,
		 * если указатель валидный, невалидный указатель не сохраняется. В режиме 
		 * отладки будет вызвано утверждение:
		 * @attention "Обработчик должен быть допустимым указателем на функцию."
		 *
		 * @param event [in] событие обработчика
		 * @param call  [in] указатель обработчика
		 */
		void add(event_type event, pointer_to_callback_function call) noexcept {
			assert(call && "Handler should be a valid function pointer");
			if(const auto id = static_cast<value_type>(event); call && !_events[id]) {
				_events[id] = call;
			}
		}

		/**
		 * @brief Возвращает указатель обработчика.
		 * 
		 * Функция возвращает указатель согласно идентификатору события. Если 
		 * для данного события не был добавлен обработчик, будет возвращен 
		 * nullptr, а режиме отладки будет вызвано утверждение:
		 * @attention "Неизвестный идентификатор события."
		 *
		 * @param event [in] cобытие обработчика
		 * @return           указатель обработчика или nullptr
		 */
		[[nodiscard]] FORCEINLINE pointer_to_callback_function call(event_type event) const
			noexcept {
			const auto id = static_cast<value_type>(event);
			const auto is_valid = id >= 0 && id < number_of_events;

			assert(is_valid && "Unknown event identifier.");
			return is_valid ? _events[id] : nullptr;
		}

	private:
		static constexpr auto number_of_events = uno::enum_traits<event_type>::size; /**< количество событий */
		core::array<pointer_to_callback_function, number_of_events> _events{};       /**< контейнер событий */
	
	}; /// !template <typename E> class table_of_callback_functions

} /// !namespace control

#endif /// !defined(TABLE_OF_POINTER_TO_CALLBACK_FUNCTION_H_FOR_THE_COMPONENT_LIBRARY_PROJECT)

