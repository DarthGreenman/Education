
/// led.h

#ifndef LIGHT_EMITTING_DIODE_H_FOR_THE_ARDUINO_UNO_PROJECT
#define LIGHT_EMITTING_DIODE_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "core/hardware.h"
#include "utilities/additional.h"
#include "utilities/common.h"

namespace uno {
	template <typename... Args>
	class light_emitting_diode;

	/********************************************************************************************************
	 * @brief    Класс для управления одним светодиодом.
	 *
	 * Класс наследует от базового класса uno::electronic_component и предоставляет интерфейс для управления
	 * состоянием светодиода, включая возможность мерцать в цифровом режиме (включение-выключение) и в режиме
	 * ШИМ (регулировка яркости); поддерживает оба типа сигналов - цифровой и аналоговый, что позволяет
	 * использовать его с различными типами светодиодов и конфигурациями подключения.
	 * @tparam S   тип сигнала - аналоговый или цифровой
	 * @tparam M   режим подключения
	 * @tparam Pin номер пина, к которому подключен светодиод
	 */
	template <typename S, typename W>
	class light_emitting_diode<S, W> :
		public uno::electronic_component<S, uno::signal::output, uno::get_bind<0, W>::value> {
		static_assert(uno::get_bind<1, W>::value == uno::color::white, "Wrong binding.");

	public:
		using basic_component = uno::electronic_component<S, uno::signal::output, uno::get_bind<0, W>::value>;
		using signal_type = typename basic_component::signal_type;

		constexpr light_emitting_diode() = default;
		light_emitting_diode(const light_emitting_diode&) = delete;
		light_emitting_diode(light_emitting_diode&&) = delete;
		~light_emitting_diode() = default;

		light_emitting_diode& operator=(const light_emitting_diode&) = delete;
		light_emitting_diode& operator=(light_emitting_diode&&) = delete;

		/****************************************************************************************************
		 * @brief    Настройка пинов - интерфейс цифрового и аналогового сигналов.
		 */
		void begin() const { basic_component::begin(); }

		/****************************************************************************************************
		 * @brief    Мигать - интерфейс цифрового сигнала.
		 * @tparam T тип сигнала - цифровой
		 */
		template<typename T = signal_type>
		FORCEINLINE typename utl::enable_if_t<uno::signal::is_digital_v<T>, void> blink() {
			_signal_value = (_signal_value == signal_type::low) ? signal_type::high : signal_type::low;
			basic_component::write(_signal_value);
		}

		/****************************************************************************************************
		 * @brief    Мигать - интерфейс аналогового сигнала.
		 * @tparam T               тип сигнала - аналоговый
		 * @param[in] signal_value интенсивность свечения, значение в интевале: 0 ...1023
		 */
		template<typename T = signal_type>
		FORCEINLINE typename utl::enable_if_t<uno::signal::is_analog_v<T>, void>
			blink(typename signal_type::value_type signal_value = signal_type::high) {
			_signal_value = (_signal_value == signal_type::low) ? (signal_value >> 2)
				: signal_type::low;
			basic_component::write(_signal_value);
		}

	private:
		typename signal_type::value_type _signal_value{};
	};

	/********************************************************************************************************
	 * @brief Класс для управления RGB-светодиодом.
	 *
	 * Класс наследует от базового класса uno::electronic_component и предоставляет интерфейс для управления
	 * цветом и режимами миганияRGB-светодиода; поддерживает оба типа сигналов - цифровой и аналоговый, что
	 * позволяет использовать его с различными типами RGB-светодиодов и конфигурациями подключения (общий анод
	 * или общий катод).
	 * @tparam S  тип сигнала - аналоговый или цифровой
	 * @tparam C  тип подключения - общий анод или общий катод
	 * @tparam Bs структура, связывающая цветовой канал светодиода с пином платы
	 */
	template <typename S, typename C, typename... Bs>
	class light_emitting_diode<S, C, Bs...> :
		public uno::electronic_component<S, uno::signal::output, uno::get_bind<0, Bs>::value...> {

		template <typename B> constexpr static bool Is_valid_bindings =
			uno::get_bind<1, B>::value == uno::color::red ||
			uno::get_bind<1, B>::value == uno::color::green ||
			uno::get_bind<1, B>::value == uno::color::blue;

		static_assert((Is_valid_bindings<Bs> && ...), "Wrong binding.");

	public:
		using basic_component =
			uno::electronic_component<S, uno::signal::output, uno::get_bind<0, Bs>::value...>;
		using signal_type = typename basic_component::signal_type;
		using common_mode = C;

		constexpr light_emitting_diode() = default;
		light_emitting_diode(const light_emitting_diode&) = delete;
		light_emitting_diode(light_emitting_diode&&) = delete;
		~light_emitting_diode() = default;

		light_emitting_diode& operator=(const light_emitting_diode&) = delete;
		light_emitting_diode& operator=(light_emitting_diode&&) = delete;

		/****************************************************************************************************
		 * @brief Настройка пинов - интерфейс цифрового и аналогового сигналов.
		 * @note  В режиме подключения с общим анодом, вначале записываем необходимое значение сигнала (в реж.
		 *        PWM - 255, цифровом - 1), а затем переключаем пин в режим OUTPUT
		 */
		void begin() const {
			if constexpr(uno::is_common_anode_v<common_mode>) {
				constexpr auto high = static_cast<typename signal_type::value_type>(
					uno::signal::is_analog_v<signal_type> ? signal_type::high >> 2 : signal_type::high);
				basic_component::write({ high, high, high });
			}
			basic_component::begin();
		}

		/****************************************************************************************************
		 * @brief  Изменить цвет - интерфейс аналогового и цифрового сигнала.
		 * @param[in] signal_value упакованное значение RGB цвета: R (16–23 bit), G (8–15 bit), B (0–7 bit)
		 */
		void change_color(uint32_t signal_value) {
			if constexpr(uno::is_common_anode_v<common_mode>) { signal_value ^= 0x00ff'ffffu; }
			const auto analog_signal_value_map = utl::decompose<typename signal_type::value_type,
				utl::log2<static_cast<uint32_t>(get_bind<1, Bs>::value & 0x0101'0101u)>::value...>
				(signal_value, 0xff);

			if constexpr(uno::signal::is_analog_v<signal_type>) {
				basic_component::write(analog_signal_value_map.value);
			} else {
				typename signal_type::value_type digital_signal_value_map[sizeof... (Bs)]{};
				convert<sizeof... (Bs)>(analog_signal_value_map.value, digital_signal_value_map);
				basic_component::write(digital_signal_value_map);
			}
		}

		/****************************************************************************************************
		 * @brief Мигать - интерфейс аналогового и цифрового сигнала
		 * @param[in] signal_value упакованное значение RGB цвета: R (16–23 bit), G (8–15 bit), B (0–7 bit)
		 */
		FORCEINLINE void blink(uint32_t signal_value = static_cast<uint32_t>(uno::color::red |
			uno::color::green | uno::color::blue)) {
			if(_is_on) {
				_is_on = false;
				change_color(static_cast<uint32_t>(0));
			} else {
				_is_on = true;
				change_color(signal_value);
			}
		}

	private:
		/****************************************************************************************************
		 * @brief Функция конвертирует массив значений аналоговых сигналов в массив значений цифровых сигналов.
		 * @tparam N                               количество элементов массива
		 * @param[in] signal_value_map_source      массив аналоговых сигналов
		 * @param[in, out] signal_value_map_target массив цифровых сигналов
		 */
		template<uint8_t N>
		FORCEINLINE	void convert(const typename signal_type::value_type(&signal_value_map_source)[N],
			typename signal_type::value_type(&signal_value_map_target)[N]) const {
			convert(signal_value_map_source, signal_value_map_target, utl::make_index_sequence<N>{});
		}
		/****************************************************************************************************
		 * @brief Функция конвертирует массив значений аналоговых сигналов в массив значений цифровых сигналов.
		 * @note  В аналоговом режиме, значение цвета для каждого канала(R, G, B) определяется интенсивностью
		 *        сигнала, который может быть в диапазоне от 0 (выключено) до 255 (максимальная яркость).
		 *        Если бит цвета установлен, канал активный, иначе неактивным.
		 *        В цифровом режиме, если бит цвета установлен, канал считается активным(включенным),
		 *        иначе - неактивным (выключенным).
		 * @tparam N                           количество элементов массива
		 * @tparam ...Is                       последовательность индексов
		 * @param[in] signal_value_map_source  массив аналоговых сигналов
		 * @param[out] signal_value_map_target массив цифровых сигналов
		 * @param index_sequence               последовательность индексов для распаковки массива и пинов
		 */
		template<uint8_t N, uint8_t... Is>
		FORCEINLINE	void convert(const typename signal_type::value_type(&signal_value_map_source)[N],
			typename signal_type::value_type(&signal_value_map_target)[N], utl::index_sequence<Is...>) const {
			((signal_value_map_target[Is] = (signal_value_map_source[Is] != 0 
				? signal_type::high : signal_type::low)), ...);
		}

		bool _is_on{};
	};

}; /// !namespace uno

#endif /// !LIGHT_EMITTING_DIODE_H_FOR_THE_ARDUINO_UNO_PROJECT


