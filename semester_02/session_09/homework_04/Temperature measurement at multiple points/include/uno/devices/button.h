/// button.h

#ifndef BUTTON_H_FOR_THE_ARDUINO_UNO_PROJECT
#define BUTTON_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <basic_types.h>
#include <callbacks.h>
#include <common.h>
#include <electronic_component.h>
#include <integrator.h>
#include <meta.h>
#include <view_interfaces.h>

#if defined(_MSC_VER)
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <stdint.h>
#endif

namespace uno {

	namespace event {
		/// Внешние события пользователя
		enum class button : uint8_t {
			pressed, /// Кнопка нажата
			released /// Кнопка отпущена
		};
	} /// !namespace event

	/**********************************************************************************************
	 * @brief Класс кнопки, который наследует от базового класса electronic_component и
	 *        предоставляет интерфейс для обработки событий нажатия и отпускания кнопки.
	 *
	 * @tparam M   направление сигнала - вход или вход с подтяжкой
	 * @tparam Pin должен быть цифровым входом, поддерживающим pull-up или pull-down резистор, в
	 *             зависимости от способа подключения кнопки к контроллеру
	 * @tparam F   стратегия антидребезга, по умолчанию "мягкая" с порогом фильтрации - 5.
	 */
	template <
		typename M,
		uint8_t Pin,
		typename P = math::integrator<typename digital_signal::value_type>,
		typename = core::enable_if_t<is_input_signal_v<M> || is_input_pullup_signal_v<M>>
	>
	class button :
		public electronic_component<digital_signal, M, Pin> {

	public:
		using event = event::button;
		using pointer_to_callback_function =
			typename control::table_of_callback_functions<event>::pointer_to_callback_function;
		using table_of_pointers_to_callback_function = control::table_of_callback_functions<event>;

		using basic_component = electronic_component<digital_signal, M, Pin>;
		using signal_type = typename basic_component::signal_type;
		using signal_mode = typename basic_component::signal_mode;
		using computing_policy = P;

		static_assert(math::is_detector_v<computing_policy, typename signal_type::value_type>,
			"In the computing policy, there must be a method called update that returns void.");

		/// Рекомендуемое значение частоты вызова планировщика
		static constexpr uint8_t default_polling_time = 5;

		constexpr button() = default;
		button(const button&) = delete;
		button(button&&) = delete;
		~button() = default;

		button& operator=(const button&) = delete;
		button& operator=(button&&) = delete;
		/******************************************************************************************
		 * @brief Инициализирует пин кнопки.
		 *
		 * Делегирует настройку режима ввода базовому классу electronic_component.
		 * В режиме input_pullup_signal активным уровнем считается LOW.
		 */
		void begin() { basic_component::begin(); }
		/******************************************************************************************
		 * @brief Регистрирует обработчик события нажатия кнопки.
		 * @param action Callback-функция, вызываемая при нажатии.
		 */
		FORCEINLINE void on_pressed(const pointer_to_callback_function action) {
			_handlers.add(event::pressed, action);
		}
		/******************************************************************************************
		 * @brief Регистрирует обработчик события отпускания кнопки.
		 * @param action Callback-функция, вызываемая при отпускании.
		 */
		FORCEINLINE void on_released(const pointer_to_callback_function action) {
			_handlers.add(event::released, action);
		}

		/******************************************************************************************
		 * @brief Обновление состояния кнопки с использованием цифровой фильтрации (Debounce).
		 *
		 * Алгоритм построен по принципу реверсивного накопительного интегратора. Частота вызова
		 * метода определяется планировщиком задач (рекомендуется каждые 5 мс).
		 */
		void update() {
			/// Проверяем изменение подтвержденного состояния
			if(_policy.update(basic_component::read()))
				finite_state_machine(_policy.value());
		}

	private:
		/******************************************************************************************
		 * @brief Конечный автомат кнопки.
		 *
		 * Получает новое подтверждённое значение сигнала после цифровой фильтрации и, при его
		 * изменении, выполняет переход внутреннего состояния кнопки.
		 * Логика автомата:
		 * - idle    + изменение сигнала -> pressed + событие pressed;
		 * - pressed + изменение сигнала -> idle    + событие released;
		 * - отсутствие изменения сигнала -> состояние сохраняется.
		 *
		 * _processed_value хранит последнее значение сигнала, уже обработанное конечным автоматом.
		 * Поэтому событие формируется только при изменении подтверждённого значения.
		 *
		 * @param signal_value Новое подтверждённое значение сигнала кнопки.
		 */
		FORCEINLINE void finite_state_machine(typename signal_type::value_type signal_value) {
			auto callback = [this](event e) {
				if(const auto function = _handlers.call(e); function)
					function();
			};

			switch(_state) {
				case fsm::idle:
					if(signal_value != _processed_value) {
						_state = fsm::active;
						callback(event::pressed);
					}
					break;
				case fsm::active:
					if(signal_value != _processed_value) {
						_state = fsm::idle;
						callback(event::released);
					}
					break;
			}
			_processed_value = signal_value;
		}

		/// Таблица колбэков (размер мин. 8 байт)
		table_of_pointers_to_callback_function _handlers{};

		/// Текущее значение сигнала
		typename signal_type::value_type _processed_value{ is_input_pullup_signal_v<signal_mode>
			? signal_type::high : signal_type::val };

		/// Политика вычисления (размер зависит от типа, 2, 4 или 8 байт...)
		computing_policy _policy{};

		/// Внутреннее состояние
		enum class fsm : uint8_t {
			idle,  /// Кнопка не активна 
			active /// Кнопка активна
		} _state{ fsm::idle }; /// Текущее состояние
	};

} /// !namespace uno

#endif /// !difined(BUTTON_H_FOR_THE_ARDUINO_UNO_PROJECT)

