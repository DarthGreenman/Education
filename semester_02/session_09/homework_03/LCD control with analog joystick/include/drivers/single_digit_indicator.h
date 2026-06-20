/// single_digit_indicator.h

#ifndef SEVEN_SEGMENT_INDICATOR_H_FOR_THE_ARDUINO_UNO_PROJECT
#define SEVEN_SEGMENT_INDICATOR_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "core/hardware.h"
#include "core/signal.h"
#include "core/types_used.h"

#include "utilities/additional.h"
#include "utilities/bitset.h"
#include "utilities/common.h"

namespace uno {

	/********************************************************************************************************
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
	template <typename C, typename... Bs>
	class single_digit_indicator :
		public uno::electronic_component<uno::signal::digital, uno::signal::output, 
		uno::get_bind<0, Bs>::value...> {

		using s = uno::single_digit_indicator_segments;
		// Проверить, что привязки для сегментов a, b, c, d, e, f, g и dp указаны правильно.
		template <typename B>
		constexpr static bool is_valid_bindings_v =
			uno::get_bind<1, B>::value == s::a ||
			uno::get_bind<1, B>::value == s::b ||
			uno::get_bind<1, B>::value == s::c ||
			uno::get_bind<1, B>::value == s::d ||
			uno::get_bind<1, B>::value == s::e ||
			uno::get_bind<1, B>::value == s::f ||
			uno::get_bind<1, B>::value == s::g ||
			uno::get_bind<1, B>::value == s::dp;

		static_assert(sizeof...(Bs) == 7 || sizeof...(Bs) == 8, "Incorrect number of pins, should be seven" 
			" or eight.");
		static_assert((is_valid_bindings_v<Bs> && ...), "Wrong binding.");

		using basic_component =
			uno::electronic_component<uno::signal::digital, uno::signal::output, uno::get_bind<0, Bs>::value...>;

	public:
		single_digit_indicator() : basic_component() { write(~encoded_high_signal); };
		single_digit_indicator(const single_digit_indicator&) = delete;
		single_digit_indicator(single_digit_indicator&&) = delete;
		~single_digit_indicator() = default;

		single_digit_indicator& operator=(const single_digit_indicator&) = delete;
		single_digit_indicator& operator=(single_digit_indicator&&) = delete;

		//////////////////// МЕТОД BEGIN ДЛЯ КЛАССА С ПЕРЕГРУЗКОЙ БАЗОВОГО ///////////////////////////////////

		/****************************************************************************************************
		 * @brief Вывод значения на семисегментный индикатор
		 * @param signal_value Упакованный символ ряда: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, b, C, d, E, F
		 */
		void show(uint32_t signal_value) { write(signal_value); }

	private:
		/****************************************************************************************************
		 * @brief  Записать сигнал в сегмент - интерфейс цифрового сигнала
		 * @param signal_value упакованное значение сигналов для сегментов
		 */
		void write(uint32_t signal_value) {
			const auto encoded_signal = uno::is_common_anode_v<C> 
				? signal_value ^ encoded_high_signal : signal_value;

			const auto map_signal_value = utl::decompose<typename basic_component::signal_type::value_type,
				utl::log2<static_cast<uint32_t>(uno::get_bind<1, Bs>::value)>::value...>(encoded_signal, 0x1);
			basic_component::write(map_signal_value.value, utl::make_index_sequence<sizeof...(Bs)>{});
		}

		constexpr static auto encoded_high_signal =
			static_cast<uint32_t>(s::a | s::b | s::c | s::d | s::e | s::f | s::g | s::dp);
	};

} /// !namespace uno

#endif /// !SEVEN_SEGMENT_INDICATOR_H_FOR_THE_ARDUINO_UNO_PROJECT

