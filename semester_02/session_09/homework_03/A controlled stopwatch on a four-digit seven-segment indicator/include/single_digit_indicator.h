/// single_digit_indicator.h

#ifndef SEVEN_SEGMENT_INDICATOR_H_FOR_THE_ARDUINO_UNO_PROJECT
#define SEVEN_SEGMENT_INDICATOR_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "bitset.h"
#include "fox.h"
#include "hardware.h"
#include "signal.h"
#include "types_used.h"

namespace uno {

	/**********************************************************************************************
	 * @brief Класс для управления одноразрядным семисегментным индикатором. Этот класс наследует от
	 * базового класса electronic_component и предоставляет интерфейс для управления состоянием сегментов
	 * индикатора, включая возможность отображения чисел и символов. Класс поддерживает оба типа сигналов
	 * - цифровой и ШИМ, что позволяет использовать его с различными типами семисегментных индикаторов и
	 * конфигурациями подключения. Класс также обеспечивает проверку правильности привязки контактов к
	 * сегментам индикатора, гарантируя, что каждый контакт соответствует одному из допустимых сегментов
	 * (a, b, c, d, e, f, g и dp).
	 * @tparam Com Общий вывод индикатора (анод или катод).
	 * @tparam ...Bs Пины, к которым подключен компонент
	 */
	template <uno::common_pin Com, typename... Bs>
	class single_digit_indicator :
		public uno::electronic_component<uno::signal::digital, uno::signal::output, uno::get_bind<0, Bs>::value...> {

		// Проверить, что привязки для сегментов a, b, c, d, e, f, g и dp указаны правильно.
		template <typename B>
		constexpr static bool Is_valid_bindings =
			uno::get_bind<1, B>::value == uno::single_digit_indicator_segments::a ||
			uno::get_bind<1, B>::value == uno::single_digit_indicator_segments::b ||
			uno::get_bind<1, B>::value == uno::single_digit_indicator_segments::c ||
			uno::get_bind<1, B>::value == uno::single_digit_indicator_segments::d ||
			uno::get_bind<1, B>::value == uno::single_digit_indicator_segments::e ||
			uno::get_bind<1, B>::value == uno::single_digit_indicator_segments::f ||
			uno::get_bind<1, B>::value == uno::single_digit_indicator_segments::g ||
			uno::get_bind<1, B>::value == uno::single_digit_indicator_segments::dp;

		static_assert(sizeof...(Bs) == 7 || sizeof...(Bs) == 8, "Incorrect number of pins, should be seven or eight.");
		static_assert((Is_valid_bindings<Bs> && ...), "Wrong binding.");

	public:
		using basic_component =
			uno::electronic_component<uno::signal::digital, uno::signal::output, uno::get_bind<0, Bs>::value...>;
		using s = uno::single_digit_indicator_segments;
		constexpr static auto encoded_high_signal =
			static_cast<uint32_t>(s::a | s::b | s::c | s::d | s::e | s::f | s::g | s::dp);

		single_digit_indicator() : basic_component() { write(~encoded_high_signal); };
		single_digit_indicator(const single_digit_indicator&) = delete;
		single_digit_indicator(single_digit_indicator&&) = delete;
		~single_digit_indicator() = default;

		single_digit_indicator& operator=(const single_digit_indicator&) = delete;
		single_digit_indicator& operator=(single_digit_indicator&&) = delete;

		/******************************************************************************************
		 * @brief Вывод значения на семисегментный индикатор
		 * @param signal_value Упакованный символ ряда: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, b, C, d, E, F
		 */
		void show(uint32_t signal_value) { write(signal_value); }

	private:
		/******************************************************************************************
		 * @brief  Записать сигнал в сегмент - интерфейс цифрового сигнала
		 * @param signal_value упакованное значение сигналов для сегментов:
		 * a (1 << 28 bit), b (1 << 24 bit), c (1 << 20 bit), d (1 << 16 bit), e (1 << 12 bit), 
		 * f (1 << 8 bit), g (1 << 4 bit), dp (1 << 0 bit);
		 */
		void write(uint32_t signal_value) {
			const auto encoded_signal = Com == uno::common_pin::anode ? signal_value ^ encoded_high_signal : signal_value;
			const auto map_signal_value = fox::decompose<typename basic_component::signal_type::value_type,
				fox::log2<static_cast<uint32_t>(uno::get_bind<1, Bs>::value)>::value...>(encoded_signal, 0x1);
			basic_component::write(map_signal_value.value, fox::make_index_sequence<sizeof...(Bs)>{});
		}
	};

} /// !namespace uno

#endif /// !SEVEN_SEGMENT_INDICATOR_H_FOR_THE_ARDUINO_UNO_PROJECT
