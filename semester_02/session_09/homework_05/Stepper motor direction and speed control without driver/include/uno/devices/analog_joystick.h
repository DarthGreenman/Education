/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * brief Драйвер аналогового джойстика.
 *
 */

#ifndef ANALOG_JOYSTICK_H_FOR_THE_ARDUINO_UNO_PROJECT
#define ANALOG_JOYSTICK_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <addons.h>
#include <basic_types.h>
#include <button.h>
#include <callbacks.h>
#include <common.h>
#include <hysteresis.h>
#include <integrator.h>
#include <log.h>
#include <meta.h>
#include <utility.h>
#include <variable_resistor.h>

#if defined(_MSC_VER)
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <stdint.h>
#endif

namespace uno {
	/**
	 * @brief Идентификаторы элементов управления аналогового джойстика.
	 *
	 * Используются при описании привязок элементов управления к выводам
	 * микроконтроллера. Позволяют проверить корректность конфигурации 
	 * устройства на этапе компиляции.
	 */
	enum class joystick_controls : uint8_t {
		button, 
		horizontal_variable_resistor, 
		vertical_variable_resistor
	};

	namespace event {
		/**
		 * @brief Внешние события.
		 *
		 * Определяют направление отклонения ручки относительно её 
		 * откалиброванного центрального положения.
		 */
		enum class movement : uint8_t {
			center,
			left,   /**< Увеличение сигнала - уменьшение сопротивления. */
			right,  /**< Уменьшение сигнала - увеличение сопротивления. */
			up,     /**< Увеличение сигнала - уменьшение сопротивления. */
			down    /**< Уменьшение сигнала - увеличение сопротивления. */
		};
	} /// !namespace event

	/*
	 * Класс объединяет кнопку и две аналоговые оси в единое устройство. 
	 * Выполняет автоматическую калибровку центрального положения, фильтрацию
	 * сигналов, определение направления отклонения и генерацию событий.
	 *
	 * @tparam C         привязка кнопки джойстика
	 * @tparam H         привязка горизонтальной оси (X)
	 * @tparam V         привязка вертикальной оси (Y)
	 * @tparam Fxy       фильтр аналоговых сигналов осей
	 * @tparam Fs        фильтр сигнала кнопки
	 * @tparam Dead_zone размер мёртвой зоны вокруг центрального положения
	 */
	template<
		typename C,
		typename H,
		typename V,
		int8_t Dead_zone = 15,
		typename Fxy = math::hysteresis<typename analog_signal::value_type>,
		typename Fs = math::integrator<typename analog_signal::value_type>
	>
	class analog_joystick {
		/// Проверяет корректность привязок элементов управления.
		using ch = uno::joystick_controls;
		static_assert(
			is_valid_bindings_v<C, ch::button>&&
			is_valid_bindings_v<H, ch::horizontal_variable_resistor>&&
			is_valid_bindings_v<V, ch::vertical_variable_resistor>,
			"Wrong binding."
			);

	public:
		using event = event::movement;
		using pointer_to_callback_function =
			typename control::table_of_callback_functions<event>::pointer_to_callback_function;
		using table_of_pointers_to_callback_function = control::table_of_callback_functions<event>;

		using button_component =
			button<input_pullup_signal, get<0, C>::value, Fs>;
		using horz_variable_resistor_component = variable_resistor<get<0, H>::value, Fxy>;
		using vert_variable_resistor_component = variable_resistor<get<0, V>::value, Fxy>;
		using signal_value_type_axis = typename analog_signal::value_type;

		static constexpr uint8_t default_polling_time = 20; /**< Рекомендуемое значение частоты вызова планировщика. */

		constexpr analog_joystick() = default;
		analog_joystick(const analog_joystick&) = delete;
		analog_joystick(analog_joystick&&) = delete;
		~analog_joystick() = default;

		analog_joystick& operator=(const analog_joystick&) = delete;
		analog_joystick& operator=(analog_joystick&&) = delete;

		/**
		 * @brief Инициализирует состояние.
		 *
		 * Метод вызывает begin() кнопки и двух осей, затем выполняет 
		 * автоматическую калибровку центра по 32 измерениям.
		 */
		void begin() {
			_button.begin();
			_axis_x.begin();
			_axis_y.begin();

			calibrate<32U>();
		}

		/**
		 * @brief Регистрирует обработчик события нажатия кнопки.
		 * @param [in] action Callback-функция, вызываемая при нажатии */
		FORCEINLINE void on_pressed(const typename button_component::pointer_to_callback_function
			action) {
			_button.on_pressed(action);
		}
		/**
		 * @brief Регистрирует обработчик события отпускания кнопки.
		 * @param [in] action Callback-функция, вызываемая при отпускании */
		FORCEINLINE void on_released(const typename button_component::pointer_to_callback_function
			action) {
			_button.on_released(action);
		}
		/**
		 * @brief Регистрирует обработчик изменения горизонтальной оси X.
		 * @param [in] action Callback-функция, вызываемая при изменении сигнала */
		FORCEINLINE void on_change_axis_x(
			const typename horz_variable_resistor_component::pointer_to_callback_function action) {
			_axis_x.on_change(action);
		}
		/**
		 * @brief Регистрирует обработчик изменения вертикальной оси Y.
		 * @param [in] action Callback-функция, вызываемая при изменении сигнала */
		FORCEINLINE void on_change_axis_y(
			const typename vert_variable_resistor_component::pointer_to_callback_function action) {
			_axis_y.on_change(action);
		}
		/**
		 * @brief Регистрирует обработчик события «влево».
		 * @param [in] action Callback-функция */
		FORCEINLINE void on_left(const pointer_to_callback_function action) {
			_handlers.add(event::left, action);
		}
		/**
		 * @brief Регистрирует обработчик события «вправо».
		 * @param [in] action Callback-функция */
		FORCEINLINE void on_right(const pointer_to_callback_function action) {
			_handlers.add(event::right, action);
		}
		/**
		 * @brief Регистрирует обработчик события «вверх».
		 * @param [in] action Callback-функция */
		FORCEINLINE void on_up(const pointer_to_callback_function action) {
			_handlers.add(event::up, action);
		}
		/**
		 * @brief Регистрирует обработчик события «вниз».
		 * @param [in] action Callback-функция */
		FORCEINLINE void on_down(const pointer_to_callback_function action) {
			_handlers.add(event::down, action);
		}
		/**
		 * @brief Обновляет состояние.
		 *
		 * Метод вызывает update() для всех компонентов, затем вычисляет 
		 * направление движения по осям и генерирует события.
		 * Частота вызова метода определяется планировщиком задач (рекомендуется
		 * каждые 20 мс)
		 */
		void update() {
			_button.update();
			_axis_x.update();
			_axis_y.update();

			move<event::right, event::left>(_axis_x.value(), _center.x, _event.x); /**< Проверяет гор. ось(Вправо / Влево) */
			move<event::down, event::up>(_axis_y.value(), _center.y, _event.y);    /**< Проверяет верт. ось(Вниз / Вверх) */
		}
		/**
		 * @brief  Возвращает текущее значение сигнала оси X.
		 * @return аналоговое значение оси X */
		[[nodiscard]] FORCEINLINE signal_value_type_axis get_signal_value_x() const noexcept {
			return _axis_x.value();
		}
		/*
		* @brief  Возвращает текущее значение сигнала оси Y.
		* @return аналоговое значение оси Y */
		[[nodiscard]] FORCEINLINE signal_value_type_axis get_signal_value_y() const noexcept {
			return _axis_y.value();
		}

	private:
		/**
		 * @brief  Выполняет калибровку центра осей.
		 * @tparam N количество измерений (8, 16 или 32) */
		template<auto N>
		FORCEINLINE void calibrate() {
			const auto [x, y] = calibrate(core::make_index_sequence<N>{});
			_center.x = x; _center.y = y;
		}

		/**
		 * @brief Выполняет автоматическую калибровку центра джойстика.
		 *
		 * Выполняет ...Is последовательных измерений каждой оси и вычисляет
		 * среднее значение. Усреднение производится делением на степень
		 * двойки посредством сдвига вправо.
		 *
		 * @tparam ...Is количество измерений (8, 16 или 32)
		 */
		template<auto... Is>
		FORCEINLINE static auto calibrate(core::index_sequence<Is...>) {
			static_assert(sizeof...(Is) == 8 || sizeof...(Is) == 16 || sizeof...(Is) == 32,
				"The number of measurements should be a multiple of 8 and no more than 32");

			constexpr auto power_of_two = math::lb<sizeof...(Is)>;
			return core::make_pair(
				(((void)Is, horz_variable_resistor_component::read()) + ...) >> power_of_two,
				(((void)Is, vert_variable_resistor_component::read()) + ...) >> power_of_two
			);
		}
		/**
		 * @brief Определяет направление движения по оси и вызывает колбэки.
		 *
		 * Если значение меньше центра — генерирует событие A.
		 * Если больше — событие B.
		 * Если равно центру — событие center.
		 *
		 * @tparam A событие при движении в отрицательную сторону
		 * @tparam B событие при движении в положительную сторону
		 * 
		 * @param [in] signal_value  текущее значение оси
		 * @param [in] center        калиброванный центр
		 * @param [in, out]cur_event текущее состояние оси (для подавления повторов)
		 */
		template<event A, event B, typename = core::enable_if_t<(A == event::right || A == event::down) &&
			(B == event::left || B == event::up)>
		>
		FORCEINLINE void move(signal_value_type_axis signal_value, signal_value_type_axis center,
			event& cur_event) {
			auto callback = [this](event e) {
				if(const auto function = _handlers.call(e); function)
					function();
			};

			if(signal_value < (center - Dead_zone) && cur_event != A) {
				cur_event = A;
				callback(cur_event);
			} else if(signal_value > (center + Dead_zone) && cur_event != B) {
				cur_event = B;
				callback(cur_event);
			} else if(signal_value == center && cur_event != event::center) {
				cur_event = event::center;
			}
		}

	private:
		/// Таблица колбэков (минимальный размер для 1 указателя - 8 байт, для 4-х 32 байта)
		table_of_pointers_to_callback_function _handlers{};

		/// 32 байта при использовании uno::hysteresis, 40 байт - uno::ema
		horz_variable_resistor_component _axis_x{};
		vert_variable_resistor_component _axis_y{};
		button_component _button{}; /// 24 байта

		/// Пересечение осей 4 байта
		struct point_in_axes {
			signal_value_type_axis x{};
			signal_value_type_axis y{};
		} _center{};

		/// События вдоль осей 2 байта
		struct events_on_axes {
			event x{ event::center };
			event y{ event::center };
		} _event{};

	};

} /// !namespace uno

#endif /// !defined(ANALOG_JOYSTICK_H_FOR_THE_ARDUINO_UNO_PROJECT)

