/// liquid_crystal_display.h


#ifndef LIQUID_CRYSTAL_DISPLAY_H_FOR_THE_ARDUINO_UNO_PROJECT
#define LIQUID_CRYSTAL_DISPLAY_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "core/hardware.h"
#include "core/types_used.h"
#include "utilities/bitset.h"

namespace uno {
	/********************************************************************************************************
	 * @brief Класс для управления жидкокристаллическим дисплеем (LCD) на базе контроллера HD44780 в 8-битном
	 *        режиме.
	 *
	 * Класс использует статический полиморфизм через шаблоны проектирования C++ для жесткой привязки
	 * выводов (pins) на этапе компиляции, что оптимизирует производительность и снижает потребление памяти.
	 * @tparam R  Тип компонента для вывода Register Select (RS).
	 * @tparam E  Тип компонента для вывода Enable (E).
	 * @tparam Bs Пакет параметров, представляющий 8 шин данных (от D0 до D7).
	 */
	template<typename R, typename E, typename... Bs>
	class liquid_crystal_display :
		public uno::electronic_component<uno::signal::digital, uno::signal::output, uno::get_bind<0, R>::value>,
		public uno::electronic_component<uno::signal::digital, uno::signal::output, uno::get_bind<0, E>::value>,
		public uno::electronic_component<uno::signal::digital, uno::signal::output,
		uno::get_bind<0, Bs>::value ...> {

		static_assert(sizeof...(Bs) == 8,
			"The number of contacts specified is incorrect, there should be eight of them. "
			"The device operates in 8-bit parallel mode.");
		/****************************************************************************************************
		 * @note Вспомогательное метавыражение для проверки валидности маппинга контактов. */
		using reg = uno::hd44780::registers;
		template <typename B>
		constexpr static bool Is_valid_bindings =
			uno::get_bind<1, R>::value == reg::rs ||
			uno::get_bind<1, E>::value == reg::e ||
			uno::get_bind<1, B>::value == reg::d0 ||
			uno::get_bind<1, B>::value == reg::d1 ||
			uno::get_bind<1, B>::value == reg::d2 ||
			uno::get_bind<1, B>::value == reg::d3 ||
			uno::get_bind<1, B>::value == reg::d4 ||
			uno::get_bind<1, B>::value == reg::d5 ||
			uno::get_bind<1, B>::value == reg::d6 ||
			uno::get_bind<1, B>::value == reg::d7;

		static_assert((Is_valid_bindings<Bs> && ...), "Wrong binding.");

		using rs_component =
			uno::electronic_component<uno::signal::digital, uno::signal::output, uno::get_bind<0, R>::value>;
		using e_component =
			uno::electronic_component<uno::signal::digital, uno::signal::output, uno::get_bind<0, E>::value>;
		using data_component =
			uno::electronic_component<uno::signal::digital, uno::signal::output, uno::get_bind<0, Bs>::value ...>;
		using signal_type = typename data_component::signal_type;

	public:
		constexpr liquid_crystal_display() = default;
		liquid_crystal_display(const liquid_crystal_display&) = delete;
		liquid_crystal_display(liquid_crystal_display&&) = delete;
		~liquid_crystal_display() = default;

		liquid_crystal_display& operator=(const liquid_crystal_display&) = delete;
		liquid_crystal_display& operator=(liquid_crystal_display&&) = delete;

		/****************************************************************************************************
		 * @brief     Инициализация дисплея. Выполняет стартовую последовательность команд HD44780.
		 * @attention Метод блокирующий, включает начальное ожидание питания и задержки между командами.
		 */
		void begin() {
			rs_component::begin();
			e_component::begin();
			data_component::begin();

			rs_component::write(signal_type::low);
			e_component::write(signal_type::low);

			delay(100); /// Ожидание стабилизации напряжения после подачи питания (15 мс)
			e_component::write(signal_type::low);
			constexpr auto size_init_sequence = sizeof(init_sequence) / sizeof(init_sequence[0]);
			init_fsm(utl::make_index_sequence<size_init_sequence>{});
		}

		/****************************************************************************************************
		 * @brief Установка позиции курсора (адреса DDRAM).
		 *
		 * Перемещает курсор в указанную позицию. Следующий выводимый символ отобразится в этих координатах.
		 * @param col Столбец (начиная с 0).
		 * @param row Строка (начиная с 0).
		 */
		FORCEINLINE void set_cursor(uint8_t col, uint8_t row) {
			/// Массив начальных смещений строк для дисплея 16x2
			constexpr uint8_t row_offsets[] = { 0x00, 0x40 };

			/// Предотвращаем выход за границы массива смещений
			if(row >= (sizeof(row_offsets) / sizeof(row_offsets[0]))) {
				row = (sizeof(row_offsets) / sizeof(row_offsets[0])) - 1;
			}

			/// Базовая команда Set DDRAM Address из даташита имеет вид: 0x80 | адрес
			uint8_t address_command = 0x80 | (col + row_offsets[row]);

			/// Отправляем как команду (RS = low)
			write(address_command, signal_type::low);

			/// Обязательная пауза на выполнение команды контроллером (минимум 37 мкс)
			delayMicroseconds(40);
		}

		/****************************************************************************************************
		 * @brief Вывод одиночного символа на дисплей.
		 * @param ch Символ для отображения.
		 */
		FORCEINLINE void print_char(char ch) {
			/// Отправляем байт символа как данные (RS = high)
			write(static_cast<uint8_t>(ch), signal_type::high);

			/// Обязательная пауза на выполнение команды контроллером (минимум 37 мкс)
			delayMicroseconds(40);
		}

	private:
		/****************************************************************************************************
		 * @brief Конечный автомат инициализации на базе развертывания шаблона пакета индексов.
		 *
		 * Использует C++17 Fold Expressions (сворачивание параметров шаблона) для последовательного вызова
		 * метода write() и задержки для каждого элемента массива init_sequence на этапе компиляции.
		 * @tparam Is Последовательность индексов для развертывания массива команд.
		 */
		template<uint8_t... Is>
		FORCEINLINE void init_fsm(utl::index_sequence<Is...>) {
			/// Выражение свёртки (Fold expression) через оператор запятая.
			/// Выполняет: write(code0, low), delay(time0), write(code1, low), delay(time1)...
			((write(init_sequence[Is].first, signal_type::low),
				delayMicroseconds(init_sequence[Is].second)), ...);
		}
		/****************************************************************************************************
		* @brief Отправка опкода (команды) в шину данных дисплея.
		*
		* Производит декомпозицию байта команды по соответствующим битам шины данных с учетом compile-time
		* маппинга портов.
		* @param code Отправляемая команда (opcode) контроллера HD44780.
		* @param mode Режим отправки (low для команд / RS=0, high для данных / RS=1).
		*/
		FORCEINLINE void write(uint8_t code, typename signal_type::value_type mode) {
			rs_component::write(mode);
			const auto signal_value_map = utl::decompose<typename signal_type::value_type,
				utl::log2<static_cast<uint32_t>(uno::get_bind<1, Bs>::value)>::value...>
				(code, 0x01);

			data_component::write(signal_value_map.value);
			pulse_enable();
			
		}
		/****************************************************************************************************
		 * @brief Формирование стробирующего импульса на выводе Enable (E).
		 *
		 * Дисплей считывает данные по спаду импульса (High -> Low).
		 */
		FORCEINLINE void pulse_enable() {
			/// Время предустановки адреса и данных (tAS по даташиту > 140 нс), перед тем как записать в E high,
			/// даем сигналам RS и шине данных полностью стабилизироваться.
			delayMicroseconds(1);

			/// Формируем передний фронт импульса стробирования
			e_component::write(signal_type::high);

			/// Ширина импульса высокого уровня (PWEH по даташиту > 450 нс)
			delayMicroseconds(1);

			/// Формируем спад строба, в эту наносекунду ЖКИ считывает данные!
			e_component::write(signal_type::low);

			/// Время удержания адреса и данных после спада (tAH, tH по даташиту > 10 нс). Важнейшая пауза:
			/// запрещаем коду менять RS и данные сразу после падения E.
			delayMicroseconds(1);
		}
		/****************************************************************************************************
		 * @brief Структура пары (Команда, Задержка в мкс) для стартовой последовательности
		 *
		 * 1-2-3. Принудительно установить 8-бит режима (0x30).
		 *     4. Настроить параметры дисплея (Function Set)  (0x38), где:
		 *                                           N = 1 (2 строки),
		 *                                           F = 0 (шрифт размером 5х8).
		 *     5. Дисплей выключить (0x08).
		 *     6. Очистить дисплей и установить адрес DDRAM равным 0 в счетчике адресов (0x01).
		 *     7. Настроить поведение курсора (0x06), где:
		 *                                           I/M = 1 (инкремент адреса, курсор движется вправо),
		 *                                           S   = 0 (без сдвига всего экрана)
		 *     8. Дисплей включить, показать курсор, мерцание выключить (0x0E).
		 * 
		 *     Время выполнения Clear Display и Return Home согласно даташиту >= 1,52 миллисекунд
		 *     Время выполнения базовых операций согласно даташиту >= 37 микросекунд		 * 
		 */
		constexpr static utl::pair<uint8_t, uint32_t> init_sequence[]{
			{ 0x30, 5000 }, /// Принудительно установить 8-бит режима (0x30)
			{ 0x30, 200 },  /// Принудительно установить 8-бит режима (0x30)
			{ 0x30, 200 },  /// Принудительно установить 8-бит режима (0x30)
			{ 0x38, 100 },  /// Настроить параметры дисплея
			{ 0x08, 100 },  /// Дисплей выключить
			{ 0x01, 3000 }, /// Очистить дисплей и установить адрес DDRAM равным 0 в счетчике адресов
			{ 0x06, 100 },  /// Настроить поведение курсора
			{ 0x0e, 100 }   /// Дисплей включить, показать курсор, мерцание выключить
		};
	};

} /// !namespace uno

#endif /// !LIQUID_CRYSTAL_DISPLAY_H_FOR_THE_ARDUINO_UNO_PROJECT

