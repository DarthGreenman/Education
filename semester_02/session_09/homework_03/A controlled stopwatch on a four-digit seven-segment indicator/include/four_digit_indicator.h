/// four_digit_indicator.h

#ifndef FOUR_DIGIT_INDICATOR_H_FOR_THE_ARDUINO_UNO_PROJECT
#define FOUR_DIGIT_INDICATOR_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "hardware.h"
#include "single_digit_indicator.h"
#include "types_used.h"

namespace uno {

	/**********************************************************************************************
	 * @brief Класс для управления четырёхразрядным семисегментным индикатором.
	 * @tparam Un Разряд единиц.
	 * @tparam Te Разряд десятков.
	 * @tparam Hu Разряд сотен.
	 * @tparam Th Разряд тысяч.
	 * @tparam ...Bs Дополнительные параметры для настройки индикатора.
	 * @tparam Com Общий вывод индикатора (анод или катод).
	 */
	template <uno::common_pin Com,
		typename Un, typename Te, typename Hu, typename Th, typename... Bs>
	class four_digit_indicator : public uno::single_digit_indicator<Com, Bs...>,
		public uno::electronic_component<uno::signal::digital, uno::signal::output,
		uno::get_bind<0, Th>::value, uno::get_bind<0, Hu>::value, uno::get_bind<0, Te>::value,
		uno::get_bind<0, Un>::value> {

		// Проверить, что привязки для разрядов единиц, десятков, сотен и тысяч указаны правильно.
		constexpr static bool Is_valid_bindings =
			uno::get_bind<1, Un>::value == uno::digit_of_the_number::units &&
			uno::get_bind<1, Te>::value == uno::digit_of_the_number::tens &&
			uno::get_bind<1, Hu>::value == uno::digit_of_the_number::hundreds &&
			uno::get_bind<1, Th>::value == uno::digit_of_the_number::thousands;

		static_assert(Is_valid_bindings, "Wrong binding.");

	public:
		using segment_component = uno::single_digit_indicator<Com, Bs...>;
		using digit_component = uno::electronic_component<
			uno::signal::digital, uno::signal::output,
			uno::get_bind<0, Th>::value,
			uno::get_bind<0, Hu>::value,
			uno::get_bind<0, Te>::value,
			uno::get_bind<0, Un>::value
		>;
		using signal_type = typename digit_component::signal_type;
		/******************************************************************************************
		 * @brief Конструктор четырёхразрядного семисегментного индикатора.
		 */
		four_digit_indicator() : segment_component(), digit_component() {
			constexpr auto idle_signal_value = Com == uno::common_pin::anode ? signal_type::high : signal_type::low;
			constexpr decltype(idle_signal_value) map_idle_signal_value[]{ idle_signal_value, idle_signal_value,
				idle_signal_value, idle_signal_value };

			// Инициализация всех разрядов дисплея в состоянии ожидания.
			digit_component::write(map_idle_signal_value, fox::make_index_sequence<digit_component::number_of_pins>{});
		}
		four_digit_indicator(const four_digit_indicator&) = delete;
		four_digit_indicator(four_digit_indicator&&) = delete;
		~four_digit_indicator() = default;

		four_digit_indicator& operator=(const four_digit_indicator&) = delete;
		four_digit_indicator& operator=(four_digit_indicator&&) = delete;

		/******************************************************************************************
		 * @brief Вывод значения на четырёхразрядный семисегментный индикатор.
		 * @param signal_value Целое число в диапазоне от 0 до 9999, которое нужно отобразить на
		 * четырёхразрядном семисегментном индикаторе.
		 */
		void show(uint32_t signal_value) {
			update(get_number_map(signal_value).value, fox::make_index_sequence<
			digit_component::number_of_pins
			>{});
		}

	private:
		/******************************************************************************************
		 * @brief Функция для получения карты цифр для отображения на четырёхразрядном дисплее.
		 * Она принимает число и возвращает массив, в котором каждый элемент соответствует одному
		 * разряду дисплея и содержит число от 0 до 9, которое должно быть отображено на этом разряде.
		 * @param signal_value Целое число в диапазоне от 0 до 9999, для которого нужно получить
		 * карту цифр. Если число превышает 9999, функция возвращает карту для отображения "----".
		 * Например для числа 1234, функция вернёт массив {4, 3, 2, 1}, где:
				- number_map[0] = 4 (единицы)
				- number_map[1] = 3 (десятки)
				- number_map[2] = 2 (сотни)
				- number_map[3] = 1 (тысячи)
		 * @return
		 */
		constexpr static decltype(auto) get_number_map(uint32_t signal_value) noexcept {
			auto tens = []() { return static_cast<uint16_t>(uno::get_bind<1, Te>::value); };
			auto hundreds = []() { return static_cast<uint16_t>(uno::get_bind<1, Hu>::value); };
			auto thousands = []() { return static_cast<uint16_t>(uno::get_bind<1, Th>::value); };

			return signal_value <= 9999
				? uno::wrapper_for_array<uint32_t, digit_component::number_of_pins>{
					{ signal_value% tens(), signal_value / tens() % tens(),
						signal_value / hundreds() % tens(), signal_value / thousands() % tens()}
			} : uno::wrapper_for_array<uint32_t, digit_component::number_of_pins>{
				{ 0xa, 0xa, 0xa, 0xa } // Отображение "----" для чисел, превышающих 9999
			};
		};
		/******************************************************************************************
		 * @brief Функция обновляет отображение на дисплее, выводя соответствующие цифры на каждый
		 * разряд в соответствии с номерной картой.
		 *
		 * @tparam N Количество разрядов в дисплее (для четырёхразрядного дисплея N будет равно 4)
		 * @tparam ...Is Пакет параметров, который используется для генерации индексов разрядов
		 * дисплея (0, 1, 2, 3 для четырёхразрядного дисплея)
		 * @param number_map Массив, в котором каждый элемент соответствует одному разряду дисплея
		 * и содержит число от 0 до 9, которое должно быть отображено на этом разряде. Например,
		 * для отображения числа 1234 на четырёхразрядном дисплее, number_map будет
		 * содержать {4, 3, 2, 1}, где:
				- number_map[0] = 4 (единицы)
				- number_map[1] = 3 (десятки)
				- number_map[2] = 2 (сотни)
				- number_map[3] = 1 (тысячи)
				Функция будет использовать эти значения для обновления каждого разряда дисплея,
				выводя соответствующую цифру на каждом из них.
		 * @param fox::index_sequence<Is...> Пакет параметров, который используется для генерации
		 * индексов разрядов дисплея (0, 1, 2, 3 для четырёхразрядного дисплея). Этот параметр
		 * позволяет функции знать, какие разряды дисплея нужно обновить и в каком порядке.
		 * Например, для четырёхразрядного дисплея, fox::index_sequence<0, 1, 2, 3> будет указывать
		 * функции обновлять разряды в следующем порядке:
				- Сначала обновить единицы (number_map[0])
				- Затем обновить десятки (number_map[1])
				- Затем обновить сотни (number_map[2])
				- Наконец, обновить тысячи (number_map[3])
		 */
		template<uint8_t N, uint8_t... Is>
		void update(const uint32_t(&number_map)[N], fox::index_sequence<Is...>) {
			auto write = [this](uint32_t number, uint8_t index) {
				constexpr auto signal = Com == uno::common_pin::anode ? signal_type::low : signal_type::high;
				// Записать сигнал в контакты разряда, который нужно обновить
				digit_component::write({ signal, signal, signal, signal }, fox::make_index_sequence<
					digit_component::number_of_pins
				>{});
				// Вывести значение на сегмент разряда, который нужно обновить
				segment_component::show(static_cast<uint32_t>(_decimal_digits[number]));
				// Сбросить сигнал на контактах разряда, который нужно обновить
				digit_component::write(~signal, index);
			};
			// Вызвать лямбду для каждого разряда, передавая соответствующее значение из number_map
			// и индекс разряда
			(write(number_map[Is], Is), ...);
		}

		using s = uno::single_digit_indicator_segments;
		constexpr static s _decimal_digits[]{
			/* 0 */ s::a | s::b | s::c | s::d | s::e | s::f,
			/* 1 */ s::b | s::c,
			/* 2 */ s::a | s::b | s::g | s::e | s::d,
			/* 3 */ s::a | s::b | s::g | s::c | s::d,
			/* 4 */ s::f | s::g | s::b | s::c,
			/* 5 */ s::a | s::f | s::g | s::c | s::d,
			/* 6 */ s::a | s::f | s::g | s::c | s::d | s::e,
			/* 7 */ s::a | s::b | s::c,
			/* 8 */ s::a | s::b | s::c | s::d | s::e | s::f | s::g,
			/* 9 */ s::a | s::b | s::c | s::d | s::f | s::g,
			/* F */ s::a | s::f | s::g | s::e
		};
	};

} /// !namespace uno

#endif // !FOUR_DIGIT_INDICATOR_H_FOR_THE_ARDUINO_UNO_PROJECT
