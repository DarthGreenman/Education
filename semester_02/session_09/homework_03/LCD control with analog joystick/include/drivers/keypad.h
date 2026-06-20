/// keypad.h

#ifndef KEYPAD_H_FOR_THE_ARDUINO_UNO_PROJECT
#define KEYPAD_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "core/hardware.h"
#include "core/types_used.h"
#include "core/ascii.h"
#include "utilities/bitset.h"
#include "utilities/callbacks_table.h"
#include "utilities/common.h"
#include "events.h"

namespace uno {

	template <typename R, typename C>
	class keypad;

	/********************************************************************************************************
	 * @brief Класс для управления мембранной клавиатурой. Этот класс наследует от базового класса
	 * electronic_component_keypad и предоставляет интерфейс для обработки событий нажатия клавиш и получения
	 * информации о том, какая клавиша была нажата. Класс также обеспечивает проверку правильности привязки
	 * контактов к строкам и столбцам клавиатуры, гарантируя, что каждый контакт соответствует одному из
	 * допустимых каналов клавиатуры (r1, r2, r 3, r4 для строк и c1 , c2, c3, c4 для столбцов).
	 * Класс поддерживает конфигурацию 4x4, что означает, что он может обрабатывать до 16 различных клавиш, 
	 * расположенных в матрице из 4 строк и 4 столбцов. Класс предоставляет методы для управления состоянием
	 * строк и столбцов клавиатуры, а также для обработки событий нажатия клавиш. Метод key_down позволяет 
	 * зарегистрировать обработчик события нажатия клавиши, который будет вызываться при обнаружении нажатия.
	 * Метод get_key возвращает символ, соответствующий последней нажатой клавише. Метод update выполняет опрос
	 * клавиатуры, управляя состоянием строк и выполняя опрос столбцов для определения, какая клавиша была нажата.
	 * @tparam R Тип компонента, представляющего строки клавиатуры
	 * @tparam C Тип компонента, представляющего столбцы клавиатуры`
	 */
	template <typename... Rs, typename... Cs>
	class keypad<utl::param_pack<Rs...>, utl::param_pack<Cs...>> :
		public uno::electronic_component<uno::signal::digital, uno::signal::output,	uno::get_bind<0, Rs>::value...>,
		public uno::electronic_component<uno::signal::digital, uno::signal::input, uno::get_bind<0, Cs>::value...> {

		/// Проверить, что привязки для строк и столбцов указаны правильно.
		template <typename B>
		constexpr static bool is_valid_bindings_v =
			uno::get_bind<1, B>::value == uno::membrane_keypad_channels::r1 ||
			uno::get_bind<1, B>::value == uno::membrane_keypad_channels::r2 ||
			uno::get_bind<1, B>::value == uno::membrane_keypad_channels::r3 ||
			uno::get_bind<1, B>::value == uno::membrane_keypad_channels::r4 ||
			uno::get_bind<1, B>::value == uno::membrane_keypad_channels::c1 ||
			uno::get_bind<1, B>::value == uno::membrane_keypad_channels::c2 ||
			uno::get_bind<1, B>::value == uno::membrane_keypad_channels::c3 ||
			uno::get_bind<1, B>::value == uno::membrane_keypad_channels::c4;

		static_assert(sizeof...(Rs) == 4 && sizeof...(Cs) == 4, "The number of contacts is incorrect, there should be four.");
		static_assert(((is_valid_bindings_v<Rs> && is_valid_bindings_v<Cs>) &&...), "Wrong binding.");

		using event = uno::event::keypad;
		using pointer_to_callback_function =
			typename uno::table_of_callback_functions<uno::event::keypad>::pointer_to_callback_function;
		using table_of_pointers_to_callback_function = uno::table_of_callback_functions<uno::event::keypad>;

		using rows_component = uno::electronic_component<uno::signal::digital, uno::signal::output, uno::get_bind<0, Rs>::value...>;
		using cols_component = uno::electronic_component<uno::signal::digital, uno::signal::input, uno::get_bind<0, Cs>::value...>;
		using signal_type = uno::signal::digital;

	public:
		/****************************************************************************************************
		 * @brief Конструктор класса keypad. Этот конструктор принимает переменное количество аргументов,
		 * которые представляют символы, соответствующие каждой клавише в матрице клавиатуры.
		 * Аргументы должны быть перечислением типа utl::ascii, и их количество должно соответствовать
		 * размеру матрицы клавиатуры (например, 16 для 4x4 клавиатуры). Конструктор инициализирует
		 * внутренние структуры данных для хранения обработчиков событий, состояния строк и столбцов
		 * клавиатуры, а также символов, соответствующих каждой клавише. Он также устанавливает
		 * начальное состояние строк клавиатуры в HIGH, что соответствует состоянию ожидания.
		 * @tparam ...K Типы аргументов, представляющих символы клавиш
		 * @param ...keys Символы клавиш
		 */
		template <typename... K>
		explicit keypad(K ...keys) : rows_component(), cols_component() {
			static_assert((utl::is_same_v<K, utl::ascii> && ...),
				"The package arguments must be an enumeration of utl::ascii.");
			static_assert(sizeof...(K) <= sizeof...(Rs) * sizeof...(Cs),
				"Number of keys must match keypad matrix size");

			constexpr typename signal_type::value_type signal_value_map[]{
				signal_type::high, signal_type::high, signal_type::high, signal_type::high };
			rows_component::write(signal_value_map, utl::make_index_sequence<sizeof...(Rs)>{}); /// Подаем на все строки HIGH

			uint8_t index{};
			[[maybe_unused]] auto assign = [this, &index](utl::ascii key) {
				const auto row = index / sizeof...(Cs);
				const auto col = index % sizeof...(Cs);
				_keys[row][col] = key;
				++index;
			};
			(assign(keys), ...); /// Назначаем имена кнопкам
		};

		keypad(const keypad&) = delete;
		keypad(keypad&&) = delete;
		~keypad() = default;

		keypad& operator=(const keypad&) = delete;
		keypad& operator=(keypad&&) = delete;

		//////////////////// МЕТОД BEGIN ДЛЯ КЛАССА С ПЕРЕГРУЗКОЙ БАЗОВОГО ///////////////////////////////////

		void key_down(const pointer_to_callback_function action) { _handlers.add(event::down, action); }
		utl::ascii get_key() const { return _key; }

		/****************************************************************************************************
		 * Алгоритм программного сканирования
		 * Конфигурация пинов:
		 *  1. Пины, подключенные к строкам, настраиваются как выходы (OUTPUT).
		 *  2. Пины, подключенные к столбцам, настраиваются как входы с подтяжкой (INPUT_PULLUP).
		 *     В режиме ожидания на всех столбцах держится высокий уровень напряжения (HIGH / 5V
		 *     или 3.3V).
		 *
		 * Микроконтроллер опрашивает клавиатуру циклически - строка за строкой.
		 * 1. Подача сигнала опроса.
		 *  1.1. Контроллер подает низкий уровень напряжения (LOW / 0V) на первую строку, оставляя остальные
		         строки в состоянии HIGH.
		 * 2. Считывание состояния столбцов.
		 *  2.1. Контроллер проверяет состояние каждого столбца.
		 *  2.2. Если ни одна кнопка в первой строке не нажата, на всех столбцах останется HIGH(INPUT_PULLUP).
		 *  2.3. Если нажать кнопку на пересечении первой строки и второго столбца, ток уйдет в землю через
		         строку, и на втором столбце зафиксируется уровень LOW.
		 * 3. Переход к следующей строке.
		 *  3.1. Контроллер возвращает первую строку в состояние HIGH, подает LOW на вторую строку и т. д. *
		 */

		 /***************************************************************************************************
		  * @brief Метод update выполняет опрос клавиатуры, управляя состоянием строк и выполняя опрос
		  * столбцов для каждой строки. Он циклически проходит по каждой строке клавиатуры, подавая на
		  * нее низкий уровень напряжения (LOW) и проверяя состояние каждого столбца.
		  * Если обнаруживается, что на каком-то столбце зафиксирован низкий уровень напряжения (LOW),
		  * это означает, что была нажата клавиша на пересечении текущей строки и этого столбца.
		  * В этом случае метод сохраняет символ, соответствующий этой клавише, и вызывает
		  * зарегистрированный обработчик события нажатия клавиши. После проверки всех столбцов для
		  * текущей строки метод возвращает эту строку в состояние высокого уровня напряжения (HIGH)
		  * и переходит к следующей строке. Этот процесс повторяется для всех строк клавиатуры,
		  * обеспечивая непрерывный мониторинг состояния клавиш.
		  */
		void update() { poll(utl::make_index_sequence<sizeof...(Rs)>{}); }

	private:
		/****************************************************************************************************
		 * @brief Опросить клавиатуру: управлять состоянием строк и выполнить опрос столбцов для каждой строки.
		 * @tparam ...Is Последовательность индексов массива строк
		 * @param handler Указатель на обработчик события
		 * @param
		 */
		template <uint8_t... Is>
		void poll(utl::index_sequence<Is...>) {
			/*
			const auto handler = _handlers.call(event::down);
			if(!handler) return;

			auto traverse = [this, handler](uint8_t row) {
				rows_component::write_new(signal_type::low, rows_component::_pins[row]); /// Подаем низкий уровень напряжения (LOW) на текущую строку
				poll(handler, row, utl::make_index_sequence<cols_component::component_map>{});
				rows_component::write_new(signal_type::high, rows_component::_pins[row]); /// Подаем высокий уровень напряжения (HIGH) на текущую строку
			};
			(traverse(Is), ...);
			*/
		}

		/****************************************************************************************************
		 * @brief Опросить столбцы для текущей строки и вызвать обработчик события, если была обнаружена нажатая
		 * клавиша.
		 * @tparam ...Is Последовательность индексов массива столбцов
		 * @param handler Указатель на обработчик события
		 * @param row Номер строки
		 * @param
		 */
		template<uint8_t... Is>
		void poll(const pointer_to_callback_function handler, uint8_t row, utl::index_sequence<Is...>) {
			/*
			if(!handler) return;

			auto traverse = [this, handler, row](uint8_t col) {
				if(!cols_component::read(cols_component::_pins[col])) {
					_key = _keys[row][col];
					handler();
				} };
			(traverse(Is), ...);
			*/
		}

		utl::ascii _keys[sizeof...(Rs)][sizeof...(Cs)]{}; /// 8 байт
		table_of_pointers_to_callback_function _handlers{}; /// 8 байт
		utl::ascii _key{ utl::ascii::null };
	};

} /// !namespace uno

#endif /// !KEYPAD_H_FOR_THE_ARDUINO_UNO_PROJECT

