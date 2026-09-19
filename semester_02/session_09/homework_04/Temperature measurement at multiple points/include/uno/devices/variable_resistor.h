/// variable_resistor.h

#ifndef VARIABLE_RESISTOR_H_FOR_THE_ARDUINO_UNO_PROJECT
#define VARIABLE_RESISTOR_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <basic_types.h>
#include <callbacks.h>
#include <common.h>
#include <electronic_component.h>
#include <hysteresis.h>
#include <view_interfaces.h>

#if defined(_MSC_VER)
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <stdint.h>
#endif


namespace uno {

	namespace event {
		enum class resistance : uint8_t { change, decreased, increased };
	} /// !namespace event

	/**********************************************************************************************
	* @brief Класс управления переменным резистором (потенциометром).
	*
	* Предоставляет интерфейс для чтения аналогового сигнала с фильтрацией шума АЦП на основе
	* стратегий.
	*
	* @tparam Pin номер аналогового пина(АЦП каналы A0 - A5, физические пины 14 - 19).
	* @tparam F   стратегия фильтрации шума. По умолчанию - фильтр гистерезиса(math::hysteresis<>).
	*/
	template<
		uint8_t Pin,
		typename P = math::hysteresis<typename analog_signal::value_type>
	>
	class variable_resistor :
		public electronic_component<analog_signal, input_signal, Pin> {

	public:
		using event = event::resistance;
		using pointer_to_callback_function =
			typename control::table_of_callback_functions<event>::pointer_to_callback_function;
		using table_of_pointers_to_callback_function = control::table_of_callback_functions<event>;

		using basic_component = electronic_component<analog_signal, input_signal, Pin>;
		using signal_type = typename basic_component::signal_type;
		using computing_policy = P;

		static_assert(
			math::is_detector_v<computing_policy, typename signal_type::value_type> ||
			math::is_transform_v<computing_policy, typename signal_type::value_type>,
			"Computing policy must have an update method returning bool or void and a value method."
			);

		/// Рекомендуемый интервал опроса АЦП планировщиком(раз в 20 мс)
		static constexpr uint8_t default_polling_time = 20;

		constexpr variable_resistor() = default;
		variable_resistor(const variable_resistor&) = delete;
		variable_resistor(variable_resistor&&) = delete;
		~variable_resistor() = default;

		variable_resistor& operator=(const variable_resistor&) = delete;
		variable_resistor& operator=(variable_resistor&&) = delete;
		/******************************************************************************************
		 * @brief Инициализирует аналоговый вход.
		 *
		 * Делегирует настройку режима базовому классу electronic_component.
		 * Дополнительной конфигурации для АЦП не требуется.
		 */
		void begin() {
			basic_component::begin();
			_processed_value = basic_component::read();
		}
		/******************************************************************************************
		 * @brief Регистрирует обработчик события изменения сопротивления.
		 *
		 * Callback вызывается только после подтверждённого изменения значения АЦП, обработанного
		 * стратегией фильтрации.
		 *
		 * @param action Указатель на функцию-обработчик.
		 */
		FORCEINLINE void on_change(const pointer_to_callback_function action) {
			_handlers.add(event::change, action);
		}

		/******************************************************************************************
		 * @brief Чтение последнего подтвержденного стабильного значения АЦП.
		 * @return значение сигнала в диапазоне 0...1023.
		 */
		[[nodiscard]] FORCEINLINE typename signal_type::value_type value() const noexcept {
			return _processed_value;
		}

		/******************************************************************************************
		 * @brief Обновляет состояние потенциометра и генерирует событие.
		 *
		 * Алгоритм:
		 * 1. Считывает текущее значение АЦП.
		 * 2. Передаёт его в стратегию фильтрации F.
		 * 3. Если фильтр подтверждает изменение — обновляет _signal_value.
		 * 4. Генерирует событие change_resistance.
		 *
		 * @note Рекомендуемая частота вызова — каждые 20 мс.
		 */
		void update() {

			const typename signal_type::value_type signal_value{ basic_component::read() };
			if constexpr(math::is_detector_v<computing_policy, typename signal_type::value_type>) {
				if(!_policy.update(signal_value))
					return;
			} else {
				_policy.update(signal_value);
			}

			if(const auto new_value = _policy.value(); new_value != _processed_value) {
				if(const auto function = _handlers.call(event::change);
					function) function();
				_processed_value = new_value;
			}
		}

	private:
		table_of_pointers_to_callback_function _handlers{}; /// Таблица колбэков (размер мин. 8 байт)
		computing_policy _policy{};  /// Политика вычисления (размер зависит от типа, 2, 4 или 8 байт...)
		typename signal_type::value_type _processed_value{};  /// Текущее значение сигнала
	};

} /// !namespace uno

#endif /// !defined(VARIABLE_RESISTOR_H_FOR_THE_ARDUINO_UNO_PROJECT)

