// joystick.h

#ifndef JOYSTICK_H_FOR_THE_ARDUINO_UNO_PROJECT
#define JOYSTICK_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "button.h"
#include "events.h"
#include "utilities/callbacks_table.h"
#include "utilities/common.h"
#include "variable_resistor.h"

namespace uno {

	template<typename C, typename H, typename V, typename Fxy = uno::filter::hysteresis<3>,
		typename Fs = uno::filter::integrator<5, filter::reset::soft>, int8_t Dead_zone = 15>
	class joystick {
		/* @note Проверить, что привязки для элементов управления указаны правильно */
		constexpr static bool is_valid_bindings_v =
			uno::get_bind<1, C>::value == uno::joystick_controls::button &&
			uno::get_bind<1, H>::value == uno::joystick_controls::horizontal_variable_resistor &&
			uno::get_bind<1, V>::value == uno::joystick_controls::vertical_variable_resistor;

		static_assert(is_valid_bindings_v, "Wrong binding.");

		using event = uno::event::joystick;
		using pointer_to_callback_function =
			typename uno::table_of_callback_functions<event>::pointer_to_callback_function;
		using table_of_pointers_to_callback_function = uno::table_of_callback_functions<event>;

		using button_component =
			uno::button<uno::signal::input_pullup, uno::get_bind<0, C>::value, Fs>;
		using horizontal_variable_resistor_component = uno::variable_resistor<uno::get_bind<0, H>::value, Fxy>;
		using vertical_variable_resistor_component = uno::variable_resistor<uno::get_bind<0, V>::value, Fxy>;
		using signal_value_type_axis = typename uno::signal::analog::value_type;

	public:
		/* @note Рекомендуемое значение частоты вызова планировщика */
		constexpr static uint8_t default_polling_time = 20;

		constexpr joystick() = default;
		joystick(const joystick&) = delete;
		joystick(joystick&&) = delete;
		~joystick() = default;

		joystick& operator=(const joystick&) = delete;
		joystick& operator=(joystick&&) = delete;

		/****************************************************************************************************
		 * @brief Инициализирует все компоненты джойстика.
		 *
		 * Вызывает begin() у кнопки и двух осей, затем выполняет автоматическую калибровку центра по N=32
		 * измерениям.
		 */
		void begin() {
			_button.begin();
			_axis_x.begin();
			_axis_y.begin();
			calibrate<32>();
		}

		/****************************************************************************************************
		 * @brief Регистрирует обработчик события нажатия кнопки.
		 * @param action Callback-функция, вызываемая при нажатии.
		 */
		void on_pressed(const typename button_component::pointer_to_callback_function action) {
			_button.on_pressed(action);
		}
		/****************************************************************************************************
		 * @brief Регистрирует обработчик события отпускания кнопки.
		 * @param action Callback-функция, вызываемая при отпускании.
		 */
		void on_released(const typename button_component::pointer_to_callback_function action) {
			_button.on_released(action);
		}
		/****************************************************************************************************
		 * @brief Регистрирует обработчик изменения горизонтальной оси X.
		 * @param action Callback-функция, вызываемая при изменении сигнала.
		 */
		void on_change_axis_x(
			const typename horizontal_variable_resistor_component::pointer_to_callback_function action) {
			_axis_x.on_change(action);
		}
		/****************************************************************************************************
		 * @brief Регистрирует обработчик изменения вертикальной оси Y.
		 * @param action Callback-функция, вызываемая при изменении сигнала.
		 */
		void on_change_axis_y(
			const typename vertical_variable_resistor_component::pointer_to_callback_function action) {
			_axis_y.on_change(action);
		}
		/****************************************************************************************************
		 * @brief Регистрирует обработчик события «влево».
		 * @param action Callback-функция.
		 */
		void on_left(const pointer_to_callback_function action) { _handlers.add(event::left, action); }
		/****************************************************************************************************
		 * @brief Регистрирует обработчик события «вправо».
		 * @param action Callback-функция.
		 */
		void on_right(const pointer_to_callback_function action) { _handlers.add(event::right, action); }
		/****************************************************************************************************
		 * @brief Регистрирует обработчик события «вверх».
		 * @param action Callback-функция.
		 */
		void on_up(const pointer_to_callback_function action) { _handlers.add(event::up, action); }
		/****************************************************************************************************
		 * @brief Регистрирует обработчик события «вниз».
		 * @param action Callback-функция.
		 */
		void on_down(const pointer_to_callback_function action) { _handlers.add(event::down, action); }
		/****************************************************************************************************
		 * @brief Обновляет состояние кнопки и осей.
		 *
		 * Вызывает update() у всех компонентов, затем вычисляет направление движения по осям и генерирует события.
		 */
		void update() {
			_button.update();
			_axis_x.update();
			_axis_y.update();

			const auto x = _axis_x.get_signal();
			const auto y = _axis_y.get_signal();

			move<event::right, event::left>(x, _center.x, _event.x); /// Проверяем горизонтальную ось (Вправо / Влево)
			move<event::down, event::up>(y, _center.y, _event.y); /// Проверяем вертикальную ось (Вниз / Вверх)
		}
		/****************************************************************************************************
		 * @brief Возвращает текущее значение сигнала оси X.
		 * @return Аналоговое значение оси X.
		 */
		[[nodiscard]] FORCEINLINE 
			signal_value_type_axis get_signal_value_x() const noexcept { return _axis_x.get_signal(); }
		/****************************************************************************************************
		* @brief Возвращает текущее значение сигнала оси Y.
		* @return Аналоговое значение оси Y.
		*/
		[[nodiscard]] FORCEINLINE 
			signal_value_type_axis get_signal_value_y() const noexcept { return _axis_y.get_signal(); }

	private:
		/****************************************************************************************************
		 * @brief Выполняет калибровку центра осей.
		 *
		 * Делает N измерений и усредняет их с помощью сдвига вправо.
		 * @tparam N Количество измерений (8, 16 или 32).
		 */
		template<uint8_t N>
		FORCEINLINE void calibrate() {
			const auto [x, y] = calibrate(utl::make_index_sequence<N>{});
			_center.x = x; _center.y = y;
		}
		/****************************************************************************************************
		 * @brief Статическая функция калибровки по index_sequence.
		 *
		 * Выполняет серию analogRead() по осям X и Y, суммирует и усредняет значения.
		 * @tparam Is   Пакет индексов.
		 * @return      Пара усреднённых значений (x, y).
		 */
		template<uint8_t... Is>
		FORCEINLINE decltype(auto) static calibrate(utl::index_sequence<Is...>) {
			static_assert(sizeof...(Is) == 8 || sizeof...(Is) == 16 || sizeof...(Is) == 32, "The number "
				"of measurements should be a multiple of 8 and no more than 32");
			constexpr auto power_of_two = utl::log2<sizeof...(Is)>::value;
			return utl::make_pair(
				(((void)Is, analogRead(uno::get_bind<0, H>::value)) + ...) >> power_of_two,
				(((void)Is, analogRead(uno::get_bind<0, V>::value)) + ...) >> power_of_two
			);
		}
		/****************************************************************************************************
		 * @brief Определяет направление движения по оси и вызывает колбэки.
		 *
		 * Если значение меньше центра — генерирует событие A.
		 * Если больше — событие B.
		 * Если равно центру — событие center.
		 * @tparam A           Событие при движении в отрицательную сторону.
		 * @tparam B           Событие при движении в положительную сторону.
		 * @param signal_value Текущее значение оси.
		 * @param center       Калиброванный центр.
		 * @param cur_event    Текущее состояние оси (для подавления повторов).
		 */
		template<event A, event B,
			typename = utl::enable_if_t<(A == event::right || A == event::down) && (B == event::left || B == event::up)>>
			FORCEINLINE void move(uint16_t signal_value, uint16_t center, event& cur_event) {
			if(signal_value < center && cur_event != A) {
				cur_event = A;
				if(const auto callback = _handlers.call(A); callback) callback();
			} else if(signal_value > center && cur_event != B) {
				cur_event = B;
				if(const auto callback = _handlers.call(B); callback) callback();
			} else if(signal_value == center && cur_event != event::center) {
				cur_event = event::center;
			}
		}

		/// Таблица колбэков (минимальный размер для 1 указателя - 8 байт, для 4-х 32 байта)
		table_of_pointers_to_callback_function _handlers{};
		/// 32 байта при использовании uno::filter::hysteresis, 40 байт - uno::filter::ema
		horizontal_variable_resistor_component _axis_x{};
		vertical_variable_resistor_component _axis_y{};
		button_component _button{}; /// 24 байта
		uno::point_in_axes _center{}; /// Пересечение осей 4 байта
		uno::event::events_on_axes _event{}; /// События вдоль осей 2 байта
	};

} /// !namespace uno

#endif /// !JOYSTICK_H_FOR_THE_ARDUINO_UNO_PROJECT

