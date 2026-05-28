/// keypad.h

#ifndef KEYPAD_H_FOR_THE_ARDUINO_UNO_PROJECT
#define KEYPAD_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "ascii.h"
#include "bitset.h"
#include "callbacks_table.h"
#include "events.h"
#include "fox.h"
#include "hardware.h"
#include "types_used.h"

namespace uno {

	/**********************************************************************************************
	 * @brief Класс для управления мембранной клавиатурой. Этот класс наследует от базового класса
	 * electronic_component и предоставляет интерфейс для обработки событий нажатия клавиш и получения
	 * информации о том, какая клавиша была нажата. Класс также обеспечивает проверку правильности
	 * привязки контактов к строкам и столбцам клавиатуры, гарантируя, что каждый контакт соответствует
	 * одному из допустимых каналов клавиатуры (r1, r2, r3, r4 для строк и c1 , c2, c3, c4 для столбцов).
	 * Класс поддерживает конфигурацию 4x4, что означает, что он может обрабатывать до 16 различных клавиш,
	 * расположенных в матрице из 4 строк и 4 столбцов.
	 * @tparam D Направление сигнала (вход или выход)
	 * @tparam ...Bs Пины, к которым подключен компонент
	 */
	template <typename D, typename... Bs>
	struct electronic_component_keypad :
		public uno::electronic_component<uno::signal::digital, D, uno::get_bind<0, Bs>::value...> {

		/// Проверить, что привязки для строк и столбцов указаны правильно.
		template <typename B>
		constexpr static bool Is_valid_bindings =
			uno::get_bind<1, B>::value == uno::membrane_keypad_channels::r1 ||
			uno::get_bind<1, B>::value == uno::membrane_keypad_channels::r2 ||
			uno::get_bind<1, B>::value == uno::membrane_keypad_channels::r3 ||
			uno::get_bind<1, B>::value == uno::membrane_keypad_channels::r4 ||
			uno::get_bind<1, B>::value == uno::membrane_keypad_channels::c1 ||
			uno::get_bind<1, B>::value == uno::membrane_keypad_channels::c2 ||
			uno::get_bind<1, B>::value == uno::membrane_keypad_channels::c3 ||
			uno::get_bind<1, B>::value == uno::membrane_keypad_channels::c4;

		static_assert(sizeof...(Bs) == 4, "The number of contacts is incorrect, there should be four.");
		static_assert((Is_valid_bindings<Bs> && ...), "Wrong binding.");

		using basic_component = uno::electronic_component<uno::signal::digital, D, uno::get_bind<0, Bs>::value...>;
		constexpr static auto component_map = sizeof...(Bs);;

		electronic_component_keypad() : basic_component() {}
		electronic_component_keypad(const electronic_component_keypad&) = delete;
		electronic_component_keypad(electronic_component_keypad&&) = delete;
		~electronic_component_keypad() = default;

		electronic_component_keypad& operator=(const electronic_component_keypad&) = delete;
		electronic_component_keypad& operator=(electronic_component_keypad&&) = delete;

		/******************************************************************************************
		 * @brief Записать массив значений сигналов в строки или столбцы клавиатуры. Этот метод
		 * позволяет записать несколько значений сигналов одновременно, что может быть полезно для
		 * управления состоянием строк или столбцов клавиатуры. Метод принимает массив значений
		 * сигналов и последовательность индексов, которая указывает, какие значения из массива
		 * должны быть записаны в соответствующие позиции строк или столбцов. Например, если компонент
		 * представляет строки клавиатуры, то можно передать массив из четырех значений сигналов
		 * (HIGH или LOW) и последовательность индексов от 0 до 3, чтобы установить состояние каждой
		 * строки клавиатуры. Если компонент представляет столбцы клавиатуры, то можно передать
		 * массив из четырех значений сигналов и последовательность индексов от 0 до 3, чтобы
		 * установить состояние каждого столбца клавиатуры.
		 * @tparam N Размер массива значений сигналов, который должен быть равен количеству контактов
		 * компонента (4 для 4x4 клавиатуры).
		 * @tparam ...I Последовательность индексов массива значений сигналов
		 * @param signal_values Массив значений сигналов
		 * @param
		 */
		template <uint8_t N, uint8_t... I>
		void write(const uint16_t(&signal_values)[N], fox::index_sequence<I...>) const {
			(basic_component::write(signal_values[I], I), ...);
		}

		/******************************************************************************************
		 * @brief Записать значение сигнала в строку или столбец клавиатуры. Этот метод позволяет
		 * записать одно значение сигнала в определенную позицию строки или столбца клавиатуры.
		 * Метод принимает значение сигнала (HIGH или LOW) и номер позиции, который указывает,
		 * в какую строку или столбец клавиатуры должно быть записано это значение. Например, если
		 * компонент представляет строки клавиатуры, то можно передать значение сигнала и номер
		 * позиции от 0 до 3, чтобы установить состояние конкретной строки клавиатуры. Если компонент
		 * представляет столбцы клавиатуры, то можно передать значение сигнала и номер позиции от
		 * 0 до 3, чтобы установить состояние конкретного столбца клавиатуры.
		 * @param signal_value Значение сигнала (HIGH или LOW)
		 * @param position Номер позиции строки или столбца
		 */
		void write(uint8_t signal_value, uint8_t position) const { basic_component::write(signal_value, position); }
		uint8_t read(uint8_t position) const { return basic_component::read(position); }
	};

	/**********************************************************************************************
	 * @brief Класс для управления мембранной клавиатурой. Этот класс наследует от базового класса
	 * electronic_component_keypad и предоставляет интерфейс для обработки событий нажатия клавиш и
	 * получения информации о том, какая клавиша была нажата. Класс также обеспечивает проверку
	 * правильности привязки контактов к строкам и столбцам клавиатуры, гарантируя, что каждый контакт
	 * соответствует одному из допустимых каналов клавиатуры (r1, r2, r 3, r4 для строк и c1 , c2,
	 * c3, c4 для столбцов). Класс поддерживает конфигурацию 4x4, что означает, что он может обрабатывать
	 * до 16 различных клавиш, расположенных в матрице из 4 строк и 4 столбцов. Класс предоставляет
	 * методы для управления состоянием строк и столбцов клавиатуры, а также для обработки событий
	 * нажатия клавиш. Метод key_down позволяет зарегистрировать обработчик события нажатия клавиши,
	 * который будет вызываться при обнаружении нажатия. Метод get_key возвращает символ, соответствующий
	 * последней нажатой клавише. Метод update выполняет опрос клавиатуры, управляя состоянием строк и
	 * выполняя опрос столбцов для определения, какая клавиша была нажата.
	 * @tparam R Тип компонента, представляющего строки клавиатуры
	 * @tparam C Тип компонента, представляющего столбцы клавиатуры
	 */
	template <typename R, typename C>
	class keypad {

	public:
		using event = uno::event::keypad;
		using pointer_to_callback_function =
			typename uno::table_of_callback_functions<uno::event::keypad>::pointer_to_callback_function;
		using table_of_pointers_to_callback_function = uno::table_of_callback_functions<uno::event::keypad>;

		using rows_type = R;
		using cols_type = C;
		using signal_type = uno::signal::digital;

		/******************************************************************************************
		 * @brief Конструктор класса keypad. Этот конструктор принимает переменное количество
		 * аргументов, которые представляют символы, соответствующие каждой клавише в матрице клавиатуры.
		 * Аргументы должны быть перечислением типа fox::ascii, и их количество должно соответствовать
		 * размеру матрицы клавиатуры (например, 16 для 4x4 клавиатуры). Конструктор инициализирует
		 * внутренние структуры данных для хранения обработчиков событий, состояния строк и столбцов
		 * клавиатуры, а также символов, соответствующих каждой клавише. Он также устанавливает
		 * начальное состояние строк клавиатуры в HIGH, что соответствует состоянию ожидания.
		 * @tparam ...K Типы аргументов, представляющих символы клавиш
		 * @param ...keys Символы клавиш
		 */
		template <typename... K>
		explicit keypad(K ...keys) : _handlers{}, _rows{}, _cols{} {
			static_assert((fox::is_same<K, fox::ascii>::value && ...), "The package arguments must be an enumeration of fox::ascii.");
			static_assert(sizeof...(K) <= rows_type::component_map * cols_type::component_map,
				"Number of keys must match keypad matrix size");

			constexpr uint16_t signal_value_map[]{ signal_type::high, signal_type::high, signal_type::high, signal_type::high };
			_rows.write(signal_value_map, fox::make_index_sequence<rows_type::component_map>{}); /// Подаем на все строки HIGH

			uint8_t index{ 0 };
			[[maybe_unused]] auto assign = [this, &index](fox::ascii key) {
				const auto row = index / cols_type::component_map;
				const auto col = index % cols_type::component_map;
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

		void key_down(const pointer_to_callback_function action) { _handlers.add(event::down, action); }
		fox::ascii get_key() const noexcept { return _key; }
		
		
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
		 *  2.2. Если ни одна кнопка в первой строке не нажата, на всех столбцах останется HIGH
		 *       (INPUT_PULLUP).
		 *  2.3. Если нажать кнопку на пересечении первой строки и второго столбца, ток уйдет в 
		 *       землю через строку, и на втором столбце зафиксируется уровень LOW.
		 * 3. Переход к следующей строке.
		 *  3.1. Контроллер возвращает первую строку в состояние HIGH, подает LOW на вторую строку
		 *       и т. д. *
		 */

		/******************************************************************************************
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
		void update() { poll(fox::make_index_sequence<rows_type::component_map>{}); }

	private:
		/******************************************************************************************
		 * @brief Опросить клавиатуру: управлять состоянием строк и выполнить опрос столбцов для
		 * каждой строки.
		 * @tparam ...Is Последовательность индексов массива строк
		 * @param handler Указатель на обработчик события
		 * @param
		 */
		template <uint8_t... Is>
		void poll(fox::index_sequence<Is...>) {
			const auto handler = _handlers.call(event::down);
			if(!handler) return;

			auto traverse = [this, handler](uint8_t row) {
				_rows.write(signal_type::low, row); /// Подаем низкий уровень напряжения (LOW) на текущую строку
				poll(handler, row, fox::make_index_sequence<cols_type::component_map>{});
				_rows.write(signal_type::high, row); /// Подаем высокий уровень напряжения (HIGH) на текущую строку
			};
			(traverse(Is), ...);
		}

		/******************************************************************************************
		 * @brief Опросить столбцы для текущей строки и вызвать обработчик события, если была
		 * обнаружена нажатая клавиша.
		 * @tparam ...Is Последовательность индексов массива столбцов
		 * @param handler Указатель на обработчик события
		 * @param row Номер строки
		 * @param
		 */
		template<uint8_t... Is>
		void poll(const pointer_to_callback_function handler, uint8_t row, fox::index_sequence<Is...>) {
			if(!handler) return;

			auto traverse = [this, handler, row](uint8_t col) {
				if(!_cols.read(col)) {
					_key = _keys[row][col];
					handler();
				} };
			(traverse(Is), ...);

		}

		fox::ascii _keys[rows_type::component_map][cols_type::component_map]{}; /// 8 байт
		table_of_pointers_to_callback_function _handlers{}; /// 8 байт
		rows_type _rows{}; /// 4 байта ???
		cols_type _cols{}; /// 4 байта ???
		fox::ascii _key{ fox::ascii::null };
	};
} /// !namespace uno

#endif /// !KEYPAD_H_FOR_THE_ARDUINO_UNO_PROJECT
