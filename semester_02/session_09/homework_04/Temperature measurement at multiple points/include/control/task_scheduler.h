/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * @brief Планировщик задач.
 *
 */

#ifndef TASK_SHEDULER_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
#define TASK_SHEDULER_H_FOR_THE_COMPONENT_LIBRARY_PROJECT

#include <common.h>
#include <meta.h>
#include <type_traits.h>

#if defined(_MSC_VER)
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <stdint.h>
#endif

namespace control {
	/**
	 * @brief Публичный интерфейс планировщика.
	 * @tparam T         тип объекта метода
	 * @tparam Poll_time интервал вызова в миллисекундах
	 * @tparam R         тип возвращаемого значения
	 * @tparam Args      типы аргументов вызываемого метода
	 */
	template <typename T, uint32_t Poll_time, typename R, typename... Args>
	class task_scheduler {
	public:
		using value_type = T;                                              /**< тип объекта */
		using pointer_to_member_function_type = R(value_type::*)(Args...); /**< тип указателя на метод объекта */

		/**
		 * @brief Реализация планировщика.
		 * @tparam Method вызываемый планировщиком метод
		 */
		template <pointer_to_member_function_type Method>
		class task_scheduler_impl {
		public:
			constexpr task_scheduler_impl() = default;
			explicit task_scheduler_impl(value_type& object) : _object{ object } {}

			task_scheduler_impl(const task_scheduler_impl&) = delete;
			task_scheduler_impl(task_scheduler_impl&&) = delete;
			~task_scheduler_impl() = default;

			task_scheduler_impl& operator=(const task_scheduler_impl&) = delete;
			task_scheduler_impl& operator=(task_scheduler_impl&&) = delete;

			/**
			 * @brief Выполняет периодический вызов метода объекта, который
			 *        возвращяет пустое значение.
			 *
			 * Функция вызывает метод объекта, если с момента предыдущего
			 * вызова прошло не менее Poll_time миллисекунд.
			 *
			 * @param uptime [in]  момент времени
			 * @param args   [in]  аргументы вызываемого метода
			 */
			template<typename U = R>
			FORCEINLINE core::enable_if_t<core::is_same_v<U, void>, void>
				exec(uint32_t uptime, Args... args) {

				if(uptime - _point_of_time >= Poll_time) {
					_point_of_time = uptime;
					(_object.*Method)(args...);
				}
			}

			/**
			 * @brief Выполняет периодический вызов метода объекта, который
			 *        возвращяет не пустое значение.
			 *
			 * Функция вызывает метод объекта, если с момента предыдущего
			 * вызова прошло не менее Poll_time миллисекунд. При первом вызове
			 * планировщика, переменной _result присваивается значение
			 * возвращенное методом объекта.
			 *
			 * @param uptime [in] момент времени
			 * @param args   [in] аргументы вызываемого метода
			 * @return            возвращает значение типа R
			 */
			template<typename U = R>
			[[nodiscard]] FORCEINLINE core::enable_if_t<!core::is_same_v<U, void>, U>
				exec(uint32_t uptime, Args... args) {

				if(!_initialized) { /**< блок вычисляется один раз при первом вызове метода */
					_initialized = true;
					_point_of_time = uptime;
					_value = (_object.*Method)(args...);
					return _value;
				}

				if(uptime - _point_of_time >= Poll_time) {
					_point_of_time = uptime;
					_value = (_object.*Method)(args...);
				}
				return _value;
			}

		private:
			value_type& _object{};     /**< ссылка на объект */
			uint32_t _point_of_time{}; /**< момент времени */
			core::conditional_t<
				core::is_same_v<R, void>, bool, R
			> _value{};  /**< возвращаемое значение, при R == void возвращаемое 
			                  значение существует только как технический 
							  заполнитель, его значение не имеет семантического смысла */
			bool _initialized{}; /**< флаг установки первого момента времени */

		}; /// !template <...> class task_scheduler_impl

	}; /// !template <...> class task_scheduler

} /// !namespace control

#endif /// !defined(TASK_SHEDULER_H_FOR_THE_COMPONENT_LIBRARY_PROJECT)

