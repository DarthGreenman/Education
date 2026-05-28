/// led_rgb.h

#ifndef LIGHT_EMITTING_DIODE_RGB_H
#define LIGHT_EMITTING_DIODE_RGB_H

#include "bitset.h"
#include "fox.h"
#include "hardware.h"
#include "signal.h"
#include "types_used.h"

#include <stdint.h>

namespace uno {
	
	/**********************************************************************************************
	 * @brief Класс для управления RGB-светодиодом. Этот класс наследует от базового класса 
	 * electronic_component и предоставляет интерфейс для управления цветом и режимами мигания 
	 * RGB-светодиода. Класс поддерживает оба типа сигналов - цифровой и ШИМ, что позволяет 
	 * использовать его с различными типами RGB-светодиодов и конфигурациями подключения (общий анод
	 * или общий катод). 
	 * @tparam S тип сигнала - ШИМ или цифровой
	 * @tparam ...Bs структура, связывающая цветовой канал светодиода с пином платы
	 * @tparam Com тип подключения - общий анод или общий катод
	 */
	template <typename S, uno::common_pin Com, typename... Bs> class light_emitting_diode_rgb :
		public uno::electronic_component<S, uno::signal::output, uno::get_bind<0, Bs>::value...> {

		template <typename B>
		constexpr static bool Is_valid_bindings =
			uno::get_bind<1, B>::value == uno::color::red || uno::get_bind<1, B>::value == uno::color::green ||
			uno::get_bind<1, B>::value == uno::color::blue;

		static_assert(sizeof...(Bs) == 3, "Incorrect number of pins, should be three.");
		static_assert(uno::signal::Is_digital<S> || uno::signal::Is_pwm<S>, "The signal must be either digital or PWM.");
		static_assert((Is_valid_bindings<Bs> && ...), "Wrong binding.");

	public:
		using basic_component = uno::electronic_component<S, uno::signal::output, uno::get_bind<0, Bs>::value...>;
		using signal_type = typename basic_component::signal_type;

		light_emitting_diode_rgb() : basic_component(),
			/* Если подлючение с общим анодом, то подаем на пины сигнал 255 | 255 | 255, иначе - 0 */
			_signal_value{ Com == uno::common_pin::anode ? high_signal_value : high_signal_value ^ high_signal_value } {
			write(_signal_value);
		}

		light_emitting_diode_rgb(const light_emitting_diode_rgb&) = delete;
		light_emitting_diode_rgb(light_emitting_diode_rgb&&) = delete;
		~light_emitting_diode_rgb() = default;

		light_emitting_diode_rgb& operator=(const light_emitting_diode_rgb&) = delete;
		light_emitting_diode_rgb& operator=(light_emitting_diode_rgb&&) = delete;

		/******************************************************************************************
		 * @brief  Изменить цвет - интерфейс сигнала ШИМ
		 * @tparam T тип сигнала - ШИМ
		 * @param signal_value упакованное значение RGB цвета: R (16–23 bit), G (8–15 bit), B (0–7 bit)
		 */
		template <typename T = signal_type> typename fox::enable_if<uno::signal::Is_pwm<T>>::type
			change_color(uint32_t signal_value) {
			write(_signal_value =
				Com == common_pin::anode ? high_signal_value ^ signal_value : signal_value
			);
		}

		/******************************************************************************************
		 * @brief  Мигать - интерфейс сигнала ШИМ
		 * @tparam T тип сигнала - ШИМ
		 * @param signal_value упакованное значение RGB цвета: R (16–23 bit), G (8–15 bit), B (0–7 bit),
		 * значение по умолчанию R = 255, G = 255, B = 255
		 */
		template <typename T = signal_type>
		typename fox::enable_if<uno::signal::Is_pwm<T>>::type blink(uint32_t signal_value = high_signal_value) {
			change_color(_signal_value =
				_signal_value != (Com == common_pin::anode ? high_signal_value ^ signal_value : signal_value)
				? signal_value : (Com == common_pin::anode ? high_signal_value ^ high_signal_value : high_signal_value)
			);
		}
		/******************************************************************************************
		 * @brief Мигать - интерфейс цифрового сигнала
		 * @tparam T тип сигнала - цифровой, значение по умолчанию 0 или 1
		 */
		template <typename T = signal_type> typename fox::enable_if<uno::signal::Is_digital<T>>::type blink() {
			write(_signal_value =
				!_signal_value ? high_signal_value : high_signal_value ^ high_signal_value
			);
		}

	private:
		/******************************************************************************************
		 * @brief  Записать интенсивность цвета - интерфейс цифрового сигнала и сигнала ШИМ
		 * @param signal_value упакованное значение RGB цвета для сигнала ШИМ: R (16–23 bit), 
		 * G (8–15 bit), B (0–7 bit);
		 * для цифрового сигнала значение 0 или 1
		 */
		void write(uint32_t signal_value) {
			const auto signal_value_map = fox::decompose<typename signal_type::value_type, 
				fox::log2<static_cast<uint32_t>(uno::get_bind<1, Bs>::value & 0x0101'0101u)>::value...>(signal_value, 0xff);
			basic_component::write(signal_value_map.value, fox::make_index_sequence<sizeof...(Bs)>{});
		}

		constexpr static auto high_signal_value =
			static_cast<uint32_t>(signal_type::high) << 16 | signal_type::high << 8 | signal_type::high;
		uint32_t _signal_value{};
	};
} /// 1namespace uno

#endif /// !LIGHT_EMITTING_DIODE_RGB_H
