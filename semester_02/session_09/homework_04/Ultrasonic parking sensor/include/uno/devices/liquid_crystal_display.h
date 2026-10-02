/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * brief Драйвер жидкокристаллического дисплея (LCD) на базе контроллера HD44780.
 *
 * Реализация драйвера для работы контроллера в 8-битном режиме.
 */

#ifndef LIQUID_CRYSTAL_DISPLAY_H_FOR_THE_ARDUINO_UNO_PROJECT
#define LIQUID_CRYSTAL_DISPLAY_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <addons.h>
#include <algorithms.h>
#include <basic_types.h>
#include <common.h>
#include <electronic_component.h>
#include <log.h>
#include <meta.h>
#include <utility.h>

#ifdef HAL
#include <arduino_time.h>
#else
#include <Arduino.h>
#endif

#if defined(_MSC_VER)
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <stdint.h>
#endif
#include <array.h>
#include <type_traits.h>

namespace uno {
	namespace hd44780 {
		/**
		 * @brief Идентификаторы элементов LCD для подключения 8-битном режиме.
		 *
		 * Используются при описании привязок элементов управления к выводам
		 * микроконтроллера. Позволяют проверить корректность конфигурации
		 * устройства на этапе компиляции.
		 */
		enum class registers : uint8_t {
			rs = 0b1111'0000, e = 0b0000'1111,
			d7 = 0b1000'0000, d6 = 0b0100'0000, d5 = 0b0010'0000, d4 = 0b0001'0000,
			d3 = 0b0000'1000, d2 = 0b0000'0100, d1 = 0b0000'0010, d0 = 0b0000'0001
		};
	}

	/*
	 * Класс наследует от базового класса uno::electronic_component и
	 * предоставляет интерфейс для управления состоянием LCD.
	 *
	 * @tparam R     иип компонента для вывода Register Select (RS)
	 * @tparam E     тип компонента для вывода Enable (E)
	 * @tparam ...Bs пакет параметров, представляющий 8 шин данных (от D0 до D7)
	 */
	template<
		typename R,
		typename E,
		typename... Bs
	>
	class liquid_crystal_display :
		public electronic_component<digital_signal, output_signal, get<0, R>::value>,
		public electronic_component<digital_signal, output_signal, get<0, E>::value>,
		public electronic_component<digital_signal, output_signal, get<0, Bs>::value ...> {

		static_assert(sizeof...(Bs) == 8,
			"The number of contacts specified is incorrect, there should be eight of them. "
			"The device operates in 8-bit parallel mode.");
		static_assert(core::is_unique_types_v<Bs...>, "The bindings are not unique."); /**< Проверяет уникальность привязок. */
		/// Проверяет корректность значений привязок.
		using reg = uno::hd44780::registers;
		static_assert(is_valid_bindings_v<R, reg::rs>, "Wrong binding.");
		static_assert(is_valid_bindings_v<E, reg::e>, "Wrong binding.");
		static_assert((is_valid_bindings_v<Bs,
			reg::d0, reg::d1, reg::d2, reg::d3, reg::d4, reg::d5, reg::d6, reg::d7> && ...),
			"Wrong binding.");

	public:
		using rs_component = electronic_component<digital_signal, output_signal, get<0, R>::value>;
		using e_component = electronic_component<digital_signal, output_signal, get<0, E>::value>;
		using data_component = electronic_component<digital_signal, output_signal, get<0, Bs>::value ...>;
		using signal_type = typename data_component::signal_type;

		constexpr liquid_crystal_display() = default;
		liquid_crystal_display(const liquid_crystal_display&) = delete;
		liquid_crystal_display(liquid_crystal_display&&) = delete;
		~liquid_crystal_display() = default;

		liquid_crystal_display& operator=(const liquid_crystal_display&) = delete;
		liquid_crystal_display& operator=(liquid_crystal_display&&) = delete;

		/**
		 * @brief Инициализирует состояние.
		 *
		 * Выполняет стартовую последовательность команд HD44780.
		 * @attention Метод блокирующий, включает начальное ожидание питания и
		 *            задержки между командами.
		 */
		void begin() {
			rs_component::begin();
			e_component::begin();
			data_component::begin();

			rs_component::write(signal_type::low);
			e_component::write(signal_type::low);

			delay(100); /**< Ожидает стабилизации напряжения после подачи питания(15 мс). */
			e_component::write(signal_type::low);
			constexpr auto size_init_sequence = sizeof(init_sequence) / sizeof(init_sequence[0]);
			init_fsm(core::make_index_sequence<size_init_sequence>{});
		}

		/**
		 * @brief Устанавлвает позицию курсора.
		 *
		 * @param [in] col номер столбца (начиная с 0)
		 * @param [in] row номер строки (начиная с 0)
		 */
		FORCEINLINE void set_cursor(uint8_t col, uint8_t row) const {
			constexpr uint8_t row_offsets[]{ 0x00, 0x40 }; /**< Массив начальных смещений строк для дисплея 16x2. */

			/// Предотвращает выход за границы массива смещений
			if(row >= (sizeof(row_offsets) / sizeof(row_offsets[0]))) {
				row = (sizeof(row_offsets) / sizeof(row_offsets[0])) - 1;
			}

			/// Базовая команда Set DDRAM Address из даташита имеет вид: 0x80 | адрес
			const uint8_t address_command{ static_cast<uint8_t>(0x80 | (col + row_offsets[row])) }; /**< Инициализирует адрес. */
			write(address_command, signal_type::low);                                               /**< Отправляет команду (RS = low). */
			delayMicroseconds(40);
		}

		FORCEINLINE void clear() const { write(0x20, signal_type::low); }

		/**
		 * @brief Отображает символ.
		 *
		 * @param [in] ch символ для отображения
		 */
		FORCEINLINE void write(char ch) const {
			write(static_cast<uint8_t>(ch), signal_type::high); /**< Отправляет байт символа как данные(RS = high). */
			delayMicroseconds(40);  /**< Выдерживает обязательную паузу на выполнение команды контроллером (минимум 37 мкс). */
		}

		/**
		 * @brief Отображает строку.
		 *
		 * @param [in] str строка для отображения
		 */
		FORCEINLINE void write(const char* str) const {
			while(*str != '\0') {
				write(*str++);
			}
		}

		/**
		 * @brief Отображает число с заданной шириной поля.
		 *
		 * @tparam T			   тип числа
		 * @param [in] value       число для отображения
		 * @param [in] field_width ширина поля для отображения числа
		 */
		template<typename T, typename = core::enable_if_t<core::is_integral_v<T> &&
			!core::is_same_v<T, bool> && !core::is_same_v<T, char>>>
			FORCEINLINE void write(T value, uint8_t field_width) const {

			const auto numeric_string = core::number_to_string(value);
			write(numeric_string, field_width);
		}

		/**
		 * @brief Отображает число с плавающей запятой с заданной точностью и шириной поля.
		 *
		 * @param [in] value       число для отображения
		 * @param [in] precision   количество знаков после запятой
		 * @param [in] field_width ширина поля для отображения числа
		 */
		FORCEINLINE void write(float value, uint8_t precision, uint8_t field_width) const {
			const auto numeric_string = core::number_to_string(value, precision);
			write(numeric_string, field_width);
		}
		
	private:
		/**
		 * @brief Конечный автомат инициализации.
		 *
		 * Использует C++17 Fold Expressions для последовательного вызова метода
		 * write() и задержки для каждого элемента массива init_sequence.
		 *
		 * @tparam ...Is         последовательность индексов для развертывания
		 *                       массива команд
		 * @param index_sequence последовательность индексов для
		 *                       распаковки массива
		 */
		template<auto... Is>
		FORCEINLINE void init_fsm(core::index_sequence<Is...>) const {
			/// Выражение свёртки (Fold expression) через оператор запятая.
			/// Выполняет: write(code0, val), delay(time0), write(code1, val), delay(time1)...
			((write(init_sequence[Is].first, signal_type::low),
				delayMicroseconds(init_sequence[Is].second)), ...);
		}

		/**
		 * @brief Отображает массив символов с заданной шириной поля.
		 *
		 * @tparam T			   тип элементов массива
		 * @tparam N			   размер массива
		 * @param [in] arr         массив символов для отображения
		 * @param [in] field_width ширина поля для отображения массива
		 */
		template<typename T, auto N>
		FORCEINLINE void write(const core::array<T, N>& arr, uint8_t field_width) const {
			const auto size = core::length(arr.data());
			const auto width = static_cast<core::pure_type_t<decltype(size)>>(field_width);
			/*
			 * TODO: Определить текущее положение курсора,
			 *       и решить "Помещается ли поле в осташееся место строки.
			 */
			if(size > width) {
				set_cursor(0, 0);
				write("ERROR: 01"); /**< Недостаточная ширина поля вывода. */
				return;
			}
			char formatted_string[max_number_of_char_in_line + 1]{};
			for(core::pure_type_t<decltype(width)> i{}; i < width; ++i) {
				formatted_string[width - i - 1] = (i < size) ? arr[size - i - 1] : ' ';
			}
			write(formatted_string);
		}

		/**
		* @brief Отправляет опкоды (команды) в шину данных.
		*
		* Производит декомпозицию байта команды по соответствующим битам шины
		* данных с учетом compile-time маппинга портов.
		*
		* @param [in] code отправляемая команда (opcode) контроллера HD44780
		* @param [in] mode режим отправки:
		*                  low  для команд / RS=0
		*                  high для данных / RS=1
		*/
		FORCEINLINE void write(uint8_t code, typename signal_type::value_type mode) const {
			rs_component::write(mode);
			const auto signal_value_map = math::decompose<typename signal_type::value_type,
				math::lb<static_cast<uint32_t>(get<1, Bs>::value)>...>
				(code, 0x01);

			data_component::write(signal_value_map());
			pulse_enable();

		}

		/**
		 * @brief Формирует стробирующий импульса на выводе Enable (E).
		 *
		 * Дисплей считывает данные по спаду импульса (High -> Low).
		 */
		FORCEINLINE void pulse_enable() const {
			delayMicroseconds(1);                  /**< Перед тем как записать high в Enable выполняет задержку. */
			e_component::write(signal_type::high); /**< Формируеn передний фронт импульса стробирования. */
			delayMicroseconds(1);                  /**< Ширина импульса высокого уровня (по даташиту > 450 нс). */
			e_component::write(signal_type::low);  /**< Формируеn спад строба, в эту наносекунду ЖКИ считывает данные. */
			delayMicroseconds(1);                  /**< Важнейшая пауза: запрещает коду менять RS и данные сразу после падения E. */
		}

		/*
		 * Структура: Команда - Задержка (мкс) для стартовой последовательности
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
		 *        Дисплей включить, скрыть курсор, мерцание выключить (0x0C).
		 *
		 *     Время выполнения Clear Display и Return Home согласно даташиту >= 1,52 миллисекунд
		 *     Время выполнения базовых операций согласно даташиту >= 37 микросекунд		 *
		 */
		static constexpr core::pair<uint8_t, uint32_t> init_sequence[]{
			{ 0x30, 5000 }, /**< Принудительно установить 8-бит режима (0x30). */
			{ 0x30, 200 },  /**< Принудительно установить 8-бит режима (0x30). */
			{ 0x30, 200 },  /**< Принудительно установить 8-бит режима (0x30). */
			{ 0x38, 100 },  /**< Настроить параметры дисплея. */
			{ 0x08, 100 },  /**< Дисплей выключить. */
			{ 0x01, 3000 }, /**< Очистить дисплей и установить адрес DDRAM равным 0 в счетчике адресов. */
			{ 0x06, 100 },  /**< Настроить поведение курсора. */
			{ 0x0c, 100 }   /**< Дисплей включить, показать курсор, мерцание выключить. */
		};

		static constexpr uint8_t max_number_of_char_in_line{ 16 };
	};

} /// !namespace uno

#endif /// !LIQUID_CRYSTAL_DISPLAY_H_FOR_THE_ARDUINO_UNO_PROJECT

