/// numeric_keypad.h

#ifndef NUMERIC_KEYPAD_H_FOR_THE_ARDUINO_UNO_PROJECT
#define NUMERIC_KEYPAD_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <addons.h>
#include <ascii.h>
#include <basic_types.h>
#include <callbacks.h>
#include <common.h>
#include <electronic_component.h>
#include <meta.h>

#if defined(_MSC_VER)
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <stdint.h>
#endif

namespace uno {

	namespace event {
		enum class key : uint8_t { up, down, hold };
	} /// !namespace event

	/**********************************************************************************************
	 * @brief Пины для подключения компонентов
	 * r1, r2, r3, r4 - строки мембранной клавиатуры
	 * c1, c2, c3, c4 - столбцы мембранной клавиатуры
	 */
	enum class
		membrane_keypad_channels : uint8_t {
		r1 = 0b1000'0000, r2 = 0b0100'0000, r3 = 0b0010'0000, r4 = 0b0001'0000,
		c1 = 0b0000'1000, c2 = 0b0000'0100, c3 = 0b0000'0010, c4 = 0b0000'0001
	};

	template <typename R, typename C>
	class numeric_keypad;

	/**********************************************************************************************
	 * @brief Класс для управления мембранной клавиатурой.
	 *
	 * Этот класс наследует от базового класса electronic_component_keypad и предоставляет интерфейс
	 * для обработки событий нажатия клавиш и получения информации о том, какая клавиша была нажата.
	 * Класс также обеспечивает проверку правильности привязки контактов к строкам и столбцам клавиатуры,
	 * гарантируя, что каждый контакт соответствует одному из допустимых каналов клавиатуры (r1, r2,
	 * r 3, r4 для строк и c1 , c2, c3, c4 для столбцов).
	 * Класс поддерживает конфигурацию 4x4, что означает, что он может обрабатывать до 16 различных
	 * клавиш, расположенных в матрице из 4 строк и 4 столбцов. Класс предоставляет методы для управления
	 * состоянием строк и столбцов клавиатуры, а также для обработки событий нажатия клавиш.
	 * Метод key_down позволяет зарегистрировать обработчик события нажатия клавиши, который будет
	 * вызываться при обнаружении нажатия.
	 * Метод get_key возвращает символ, соответствующий последней нажатой клавише.
	 * Метод update выполняет опрос клавиатуры, управляя состоянием строк и выполняя опрос столбцов для
	 * определения, какая клавиша была нажата.
	 *
	 * @tparam R Тип компонента, представляющего строки клавиатуры
	 * @tparam C Тип компонента, представляющего столбцы клавиатуры`
	 */
	template <
		typename... Rs,
		typename... Cs
	>
	class numeric_keypad<core::package_of_types<Rs...>, core::package_of_types<Cs...>> :
		public electronic_component<digital_signal, output_signal, get<0, Rs>::value...>,
		public electronic_component<digital_signal, input_pullup_signal, get<0, Cs>::value...> {

		static_assert(sizeof...(Rs) == 4 && sizeof...(Cs) == 4, "The number of contacts is incorrect, "
			"there should be four.");
		/// Проверяем уникальность привязок.
		static_assert(core::is_unique_types_v<Rs...>, "The bindings are not unique.");
		static_assert(core::is_unique_types_v<Cs...>, "The bindings are not unique.");

		/// Проверяем корректность привязки строк и столбцов.
		using ch = uno::membrane_keypad_channels;
		static_assert((is_valid_bindings_v<Rs, ch::r1, ch::r2, ch::r3, ch::r4> && ...),
			"Wrong binding.");
		static_assert((is_valid_bindings_v<Cs, ch::c1, ch::c2, ch::c3, ch::c4> && ...),
			"Wrong binding.");

	public:
		using event = event::key;
		using pointer_to_callback_function =
			typename control::table_of_callback_functions<event>::pointer_to_callback_function;
		using table_of_pointers_to_callback_function = control::table_of_callback_functions<event>;

		using signal_type = digital_signal;
		using rows_component = electronic_component<signal_type, output_signal, get<0, Rs>::value...>;
		using cols_component = electronic_component<signal_type, input_pullup_signal, get<0, Cs>::value...>;

		constexpr numeric_keypad() = default;
		numeric_keypad(const numeric_keypad&) = delete;
		numeric_keypad(numeric_keypad&&) = delete;
		~numeric_keypad() = default;

		numeric_keypad& operator=(const numeric_keypad&) = delete;
		numeric_keypad& operator=(numeric_keypad&&) = delete;

		template <typename... Ks>
		constexpr void assing(Ks ...keys) {
			static_assert((core::is_same_v<Ks, core::ascii> && ...), "The package arguments must be "
				"an enumeration of core::ascii.");
			static_assert(sizeof...(Ks) <= sizeof...(Rs) * sizeof...(Cs), "Number of keys must match "
				"numeric_keypad matrix size");

			uint8_t index{};
			auto assign_impl = [this, &index](core::ascii key) {
				const auto row = index / sizeof...(Cs);
				const auto col = index % sizeof...(Cs);
				_keys[row][col] = key;
				++index;
			};
			(assign_impl(keys), ...); /// Назначаем имена кнопкам
		};

		/******************************************************************************************
		 * @brief Метод begin инициализирует клавиатуру.
		 */
		void begin() {
			rows_component::begin();
			cols_component::begin();
			rows_component::write(signal_type::high); /// Подаем на все строки HIGH
		}

		FORCEINLINE void key_down(const pointer_to_callback_function action) {
			_handlers.add(event::down, action);
		}
		[[nodiscard]] FORCEINLINE core::ascii get_key() const { return _key; }

		/******************************************************************************************
		 * Алгоритм программного сканирования
		 * Конфигурация пинов:
		 *  1. Пины, подключенные к строкам, настраиваются как выходы (OUTPUT).
		 *  2. Пины, подключенные к столбцам, настраиваются как входы с подтяжкой (INPUT_PULLUP).
		 *     В режиме ожидания на всех столбцах держится высокий уровень напряжения (HIGH / 5V
		 *     или 3.3V).
		 *
		 * Микроконтроллер опрашивает клавиатуру циклически - строка за строкой.
		 * 1. Подача сигнала опроса.
		 *  1.1. Контроллер подает низкий уровень напряжения (LOW / 0V) на первую строку, оставляя
		 *       остальные строки в состоянии HIGH.
		 * 2. Считывание состояния столбцов.
		 *  2.1. Контроллер проверяет состояние каждого столбца.
		 *  2.2. Если ни одна кнопка в первой строке не нажата, на всех столбцах останется
		 *       HIGH(INPUT_PULLUP).
		 *  2.3. Если нажать кнопку на пересечении первой строки и второго столбца, ток уйдет в землю
		 *       через строку, и на втором столбце зафиксируется уровень LOW.
		 * 3. Переход к следующей строке.
		 *  3.1. Контроллер возвращает первую строку в состояние HIGH, подает LOW на вторую строку
		 *       и т. д. *
		 */

		 /*****************************************************************************************
		  * @brief Метод update.
		  *
		  * Выполняет опрос клавиатуры, управляя состоянием строк и выполняя опрос столбцов для
		  * каждой строки. Он циклически проходит по каждой строке клавиатуры, подавая на нее низкий
		  * уровень напряжения (LOW) и проверяя состояние каждого столбца. Если обнаруживается, что
		  * на каком-то столбце зафиксирован низкий уровень напряжения (LOW), это означает, что была
		  * нажата клавиша на пересечении текущей строки и этого столбца. В этом случае метод
		  * сохраняет символ, соответствующий этой клавише, и вызывает зарегистрированный обработчик
		  * события нажатия клавиши. После проверки всех столбцов для текущей строки метод возвращает
		  * эту строку в состояние высокого уровня напряжения (HIGH) и переходит к следующей строке.
		  * Этот процесс повторяется для всех строк клавиатуры, обеспечивая непрерывный мониторинг
		  * состояния клавиш.
		  */
		void update() {
			scan(core::make_index_sequence<sizeof...(Rs)>{});
		}

	private:

		/******************************************************************************************
		 * @brief Опросить клавиатуру: управлять состоянием строк и выполнить опрос столбцов для
		 *        каждой строки.
		 *
		 * @tparam ...Is Последовательность индексов массива строк
		 */
		template <auto... Is>
		void scan(core::index_sequence<Is...>) {
			auto traverse = [this](uint8_t pin_number, uint8_t row) {
				/// Подаем низкий уровень напряжения (LOW) на текущую строку
				rows_component::write(signal_type::val, pin_number);
				/// Опрашиваем столбцы
				scan(row, core::make_index_sequence<sizeof...(Cs)>{});
				/// Подаем высокий уровень напряжения (HIGH) на текущую строку
				rows_component::write(signal_type::high, pin_number);
			};
			(traverse(get<0, Rs>::value, Is), ...);
		}

		/******************************************************************************************
		 * @brief Опросить столбцы для текущей строки и вызвать обработчик события, если была
		 *        обнаружена нажатая клавиша.
		 *
		 * @tparam ...Is  Последовательность индексов массива столбцов
		 * @param handler Указатель на обработчик события
		 * @param row     Номер строки
		 */
		template<auto... Is>
		void scan(uint8_t row, core::index_sequence<Is...>) {
			auto traverse = [this, row](uint8_t pin_number, uint8_t col) {
				if(!cols_component::read(pin_number))
					_key = _keys[row][col];
			};
			(traverse(get<0, Cs>::value, Is), ...);
		}

	private:
		core::ascii _keys[sizeof...(Rs)][sizeof...(Cs)]{}; /// 8 байт
		table_of_pointers_to_callback_function _handlers{}; /// 8 байт
		core::ascii _key{ core::ascii::null }; /// 1 байт
	};

} /// !namespace uno

#endif /// !defined(NUMERIC_KEYPAD_H_FOR_THE_ARDUINO_UNO_PROJECT)

/*
template <uint64_t... Is>
void scan(core::index_sequence<Is...>) {
	const auto handler = _handlers.call(event::down);
	if(!handler) return;

	auto traverse = [this, handler](uint8_t pin_number, uint8_t row) {
		/// Подаем низкий уровень напряжения (LOW) на текущую строку
		rows_component::write(signal_type::val, pin_number);
		/// Опрашиваем столбцы
		scan(handler, row, core::make_index_sequence<sizeof...(Cs)>{});
		/// Подаем высокий уровень напряжения (HIGH) на текущую строку
		rows_component::write(signal_type::high, pin_number);
	};
	(traverse(uno::get<0, Rs>::value, Is), ...);

template<uint64_t... Is>
void scan(const pointer_to_callback_function handler, uint8_t row, core::index_sequence<Is...>) {
	if(!handler) return;

	auto traverse = [this, handler, row](uint8_t pin_number, uint8_t col) {
		if(!cols_component::read(pin_number)) {
			_key = _keys[row][col];
			handler();
		} };
	(traverse(uno::get<0, Cs>::value, Is), ...);
}
}
*/