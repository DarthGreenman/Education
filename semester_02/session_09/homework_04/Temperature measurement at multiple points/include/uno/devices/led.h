
/// led.h

#ifndef LIGHT_EMITTING_DIODE_H_FOR_THE_ARDUINO_UNO_PROJECT
#define LIGHT_EMITTING_DIODE_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <addons.h>
#include <algorithms.h>
#include <basic_types.h>
#include <common.h>
#include <common_pin.h>
#include <electronic_component.h>
#include <log.h>
#include <meta.h>
#include <type_traits.h>

#if defined(_MSC_VER)
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <stdint.h>
#endif


namespace uno {

	/**********************************************************************************************
	 * @brief Цвета для RGB-светодиода
	 * 31–24 - альфа-канал
	 * 23-16 - red  15-8 - green  7-0 - blue */
	enum class color : uint32_t {
		red = 0x00ff'0000, green = 0x0000'ff00, blue = 0x0000'00ff
	};

	/**********************************************************************************************
	 * Операторы для перечислений, которые позволяют использовать их значения в битовых операциях
	 * (|, ^, &, ~) и сравнениях (==, !=) без необходимости явного приведения типов.
	 */
	FORCEINLINE constexpr color operator|(color lhs, color rhs) noexcept {
		return static_cast<color>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
	}
	template<typename T, typename = core::enable_if_t<core::is_integral_v<T>>>
	FORCEINLINE constexpr color operator|(color lhs, T rhs) noexcept {
		return static_cast<color>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
	}
	template<typename T, typename = core::enable_if_t<core::is_integral_v<T>>>
	FORCEINLINE constexpr color operator|(T lhs, color rhs) noexcept {
		return static_cast<color>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
	}

	FORCEINLINE constexpr color operator^(color lhs, color rhs) noexcept {
		return static_cast<color>(static_cast<uint32_t>(lhs) ^ static_cast<uint32_t>(rhs));
	}
	template<typename T, typename = core::enable_if_t<core::is_integral_v<T>>>
	FORCEINLINE constexpr color operator^(color lhs, T rhs) noexcept {
		return static_cast<color>(static_cast<uint32_t>(lhs) ^ static_cast<uint32_t>(rhs));
	}
	template<typename T, typename = core::enable_if_t<core::is_integral_v<T>>>
	FORCEINLINE constexpr color operator^(T lhs, color rhs) noexcept {
		return static_cast<color>(static_cast<uint32_t>(lhs) ^ static_cast<uint32_t>(rhs));
	}

	FORCEINLINE constexpr color operator&(color lhs, color rhs) noexcept {
		return static_cast<color>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
	}
	template<typename T, typename = core::enable_if_t<core::is_integral_v<T>>>
	FORCEINLINE constexpr color operator&(color lhs, T rhs) noexcept {
		return static_cast<color>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
	}
	template<typename T, typename = core::enable_if_t<core::is_integral_v<T>>>
	FORCEINLINE constexpr color operator&(T lhs, color rhs) noexcept {
		return static_cast<color>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
	}


	template <typename... Args>
	class light_emitting_diode;

	/**********************************************************************************************
	 * @brief Класс для управления одним светодиодом.
	 *
	 * Класс наследует от базового класса uno::electronic_component и предоставляет интерфейс для
	 * управления состоянием светодиода, включая возможность мерцать в цифровом режиме (включение-
	 * выключение) и в режиме ШИМ (регулировка яркости); поддерживает оба типа сигналов - цифровой
	 * и аналоговый, что позволяет использовать его с различными типами светодиодов и конфигурациями
	 * подключения.
	 *
	 * @tparam S   тип сигнала - аналоговый или цифровой
	 * @tparam W   режим подключения
	 * @tparam Pin номер пина, к которому подключен светодиод
	 */
	template <
		typename S,
		typename W
	>
	class light_emitting_diode<S, W> :
		public electronic_component<S, output_signal, get<0, W>::value> {
		static_assert(!(get<1, W>::value), "Wrong binding.");

	public:
		using basic_component = electronic_component<S, output_signal, get<0, W>::value>;
		using signal_type = typename basic_component::signal_type;

		constexpr light_emitting_diode() = default;
		light_emitting_diode(const light_emitting_diode&) = delete;
		light_emitting_diode(light_emitting_diode&&) = delete;
		~light_emitting_diode() = default;

		light_emitting_diode& operator=(const light_emitting_diode&) = delete;
		light_emitting_diode& operator=(light_emitting_diode&&) = delete;

		/******************************************************************************************
		 * @brief Настройка пинов - интерфейс цифрового сигнала. */
		template<typename T = signal_type>
		core::enable_if_t<is_digital_signal_v<T>, void>
			begin() const { basic_component::begin(); }

		/******************************************************************************************
		 * @brief Мигать - интерфейс цифрового сигнала.
		 *
		 * @tparam T тип сигнала - цифровой
		 */
		template<typename T = signal_type>
		FORCEINLINE core::enable_if_t<is_digital_signal_v<T>, void> blink() {
			_signal_value = (_signal_value == signal_type::val) ? signal_type::high : signal_type::val;
			basic_component::write(_signal_value);
		}

		/******************************************************************************************
		 * @brief Мигать - интерфейс аналогового сигнала.
		 *
		 * @tparam T               тип сигнала - аналоговый
		 * @param[in] signal_value интенсивность свечения, значение в интевале: 0 ...1023
		 */
		template<typename T = signal_type>
		FORCEINLINE core::enable_if_t<is_analog_signal_v<T>, void> blink() {
			_signal_value = (_signal_value == signal_type::val) ? (signal_type::high >> 2)
				: signal_type::val;
			basic_component::write(_signal_value);
		}

	private:
		typename signal_type::value_type _signal_value{};
	};

	/**********************************************************************************************
	 * @brief Класс для управления RGB-светодиодом.
	 *
	 * Класс наследует от базового класса uno::electronic_component и предоставляет интерфейс для
	 * управления цветом и режимами миганияRGB-светодиода; поддерживает оба типа сигналов - цифровой
	 * и аналоговый, что позволяет использовать его с различными типами RGB-светодиодов и
	 * конфигурациями подключения (общий анод или общий катод).
	 *
	 * @tparam S  тип сигнала - аналоговый или цифровой
	 * @tparam C  тип подключения - общий анод или общий катод
	 * @tparam Bs структура, связывающая цветовой канал светодиода с пином платы
	 */
	template <
		typename S,
		typename C,
		typename... Bs
	>
	class light_emitting_diode<S, C, Bs...> :
		public electronic_component<S, output_signal, get<0, Bs>::value...> {

		static_assert(sizeof...(Bs) == 3, "The number of contacts is incorrect, there should be three.");
		/// Проверяем уникальность привязок.
		static_assert(core::is_unique_types_v<Bs...>, "The bindings are not unique.");
		/// Проверяем корректность значений привязок.
		static_assert((is_valid_bindings_v<Bs, color::red, color::green, color::blue>
			&& ...), "Wrong binding.");

	public:
		using basic_component = electronic_component<S, output_signal, get<0, Bs>::value...>;
		using signal_type = typename basic_component::signal_type;
		using common_mode = C;

		constexpr light_emitting_diode() = default;
		light_emitting_diode(const light_emitting_diode&) = delete;
		light_emitting_diode(light_emitting_diode&&) = delete;
		~light_emitting_diode() = default;

		light_emitting_diode& operator=(const light_emitting_diode&) = delete;
		light_emitting_diode& operator=(light_emitting_diode&&) = delete;

		/******************************************************************************************
		 * @brief Настройка пинов - интерфейс цифрового и аналогового сигналов.
		 *
		 * В режиме подключения с общим анодом, вначале записываем необходимое значение сигнала
		 * (в реж. PWM - 255, цифровом - 1), а затем переключаем пин в режим OUTPUT только для
		 * цифрового сигнала.
		 */
		void begin() const {
			if constexpr(is_digital_signal_v<signal_type>)
				basic_component::begin();

			if constexpr(is_common_anode_v<common_mode>) {
				constexpr auto high = static_cast<typename signal_type::value_type>(
					is_analog_signal_v<signal_type> ? signal_type::high >> 2 : signal_type::high);
				basic_component::write({ high, high, high });
			}
		}

		/******************************************************************************************
		 * @brief Изменить цвет - интерфейс аналогового и цифрового сигнала.
		 *
		 * @param[in] signal_value упакованное значение RGB цвета: R (16–23 bit), G (8–15 bit),
		 *                         B (0–7 bit)
		 */
		void change_color(uint32_t signal_value) {
			if constexpr(is_common_anode_v<common_mode>) { signal_value ^= 0x00ff'ffffu; }
			const auto analog_signal_value_map = math::decompose<typename analog_signal::value_type,
				math::lb<static_cast<uint32_t>(get<1, Bs>::value & 0x0101'0101)>...>
				(signal_value, 0xff);

			if constexpr(is_analog_signal_v<signal_type>) {
				basic_component::write(analog_signal_value_map());
			} else {
				typename uno::digital_signal::value_type digital_signal_value_map[sizeof... (Bs)]{};
				convert<sizeof... (Bs)>(analog_signal_value_map(), digital_signal_value_map);
				basic_component::write(digital_signal_value_map);
			}
		}

		/******************************************************************************************
		 * @brief Мигать - интерфейс аналогового и цифрового сигнала.
		 *
		 * @param[in] signal_value упакованное значение RGB цвета: R (16–23 bit), G (8–15 bit),
		 *                         B (0–7 bit)
		 */
		FORCEINLINE void blink(uint32_t signal_value = static_cast<uint32_t>(color::red |
			color::green | color::blue)) {
			if(_is_on) {
				_is_on = false;
				change_color(0);
			} else {
				_is_on = true;
				change_color(signal_value);
			}
		}

	private:
		/******************************************************************************************
		 * @brief Функция конвертирует массив значений аналоговых сигналов в массив значений
		 *        цифровых сигналов.
		 *
		 * @tparam B                               количество элементов массива
		 * @param[in] signal_value_map_source      массив аналоговых сигналов
		 * @param[in, out] signal_value_map_target массив цифровых сигналов
		 */
		template<auto N>
		FORCEINLINE	void convert(const typename uno::analog_signal::value_type(&signal_value_map_source)[N],
			typename digital_signal::value_type(&signal_value_map_target)[N]) const {
			convert(signal_value_map_source, signal_value_map_target, core::make_index_sequence<N>{});
		}
		/******************************************************************************************
		 * @brief Функция конвертирует массив значений аналоговых сигналов в массив значений
		 *        цифровых сигналов.
		 *
		 * В аналоговом режиме, значение цвета для каждого канала(R, G, B) определяется интенсивностью
		 * сигнала, который может быть в диапазоне от 0 (выключено) до 255 (максимальная яркость).
		 * Если бит цвета установлен, канал активный, иначе неактивным.
		 * В цифровом режиме, если бит цвета установлен, канал считается активным(включенным),
		 * иначе - неактивным (выключенным).
		 *
		 * @tparam B                           количество элементов массива
		 * @tparam ...Is                       последовательность индексов
		 * @param[in] signal_value_map_source  массив аналоговых сигналов
		 * @param[out] signal_value_map_target массив цифровых сигналов
		 * @param index_sequence               последовательность индексов для распаковки массива и пинов
		 */
		template<auto N, auto... Is>
		FORCEINLINE	void convert(const typename uno::analog_signal::value_type(&signal_value_map_source)[N],
			typename digital_signal::value_type(&signal_value_map_target)[N], core::index_sequence<Is...>) const {
			((signal_value_map_target[Is] = (signal_value_map_source[Is] != 0
				? digital_signal::high : digital_signal::val)), ...);
		}

	private:
		bool _is_on{};
	};

}; /// !namespace uno

#endif /// !defined(LIGHT_EMITTING_DIODE_H_FOR_THE_ARDUINO_UNO_PROJECT)


