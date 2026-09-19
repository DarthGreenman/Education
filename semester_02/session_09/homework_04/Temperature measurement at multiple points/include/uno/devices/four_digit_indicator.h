/// four_digit_indicator.h

#ifndef FOUR_DIGIT_INDICATOR_H_FOR_THE_ARDUINO_UNO_PROJECT
#define FOUR_DIGIT_INDICATOR_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <addons.h>
#include <basic_types.h>
#include <common.h>
#include <common_pin.h>
#include <electronic_component.h>
#include <meta.h>
#include <single_digit_indicator.h>
#include <utility.h>

#if defined(_MSC_VER)
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <stdint.h>
#endif

namespace uno {

	/**********************************************************************************************
	 * @brief Весовые коэффициенты десятичных разрядов.
	 *
	 * Используются при разложении целого числа на отдельные цифры для отображения
	 * на четырёхразрядном семисегментном индикаторе.
	 */
	enum class
		digit_of_the_number : uint16_t {
		units = 0x0001, tens = 0x000a, hundreds = 0x0064, thousands = 0x3e8
	};

	/**********************************************************************************************
	 * @brief Класс для управления четырёхразрядным семисегментным индикатором.
	 * @tparam C     Общий вывод индикатора (анод или катод).
	 * @tparam Un    Разряд единиц.
	 * @tparam Te    Разряд десятков.
	 * @tparam Hu    Разряд сотен.
	 * @tparam Th    Разряд тысяч.
	 * @tparam ...Bs Дополнительные параметры для настройки индикатора.
	 */
	template <
		typename C,
		typename Un,
		typename Te,
		typename Hu,
		typename Th,
		typename... Bs
	>
	class four_digit_indicator :
		public single_digit_indicator<C, Bs...>,
		public electronic_component<
		digital_signal,
		output_signal,
		get<0, Th>::value,
		get<0, Hu>::value,
		get<0, Te>::value,
		get<0, Un>::value> {

		/// Проверяем корректность привязок разрядов единиц, десятков, сотен и тысяч.
		using dig = digit_of_the_number;
		static_assert(
			is_valid_bindings_v<Un, dig::units>&&
			is_valid_bindings_v<Te, dig::tens>&&
			is_valid_bindings_v<Hu, dig::hundreds>&&
			is_valid_bindings_v<Th, dig::thousands>,
			"Wrong binding."
			);

	public:
		using common_mode = C;
		using segment_component = single_digit_indicator<C, Bs...>;
		using digit_component = electronic_component<
			digital_signal,
			output_signal,
			get<0, Th>::value,
			get<0, Hu>::value,
			get<0, Te>::value,
			get<0, Un>::value>;
		using signal_type = typename digit_component::signal_type;

		constexpr four_digit_indicator() = default;
		four_digit_indicator(const four_digit_indicator&) = delete;
		four_digit_indicator(four_digit_indicator&&) = delete;
		~four_digit_indicator() = default;

		four_digit_indicator& operator=(const four_digit_indicator&) = delete;
		four_digit_indicator& operator=(four_digit_indicator&&) = delete;

		/******************************************************************************************
		 * @brief Инициализирует четырёхразрядный индикатор.
		 *
		 * Выполняет инициализацию сегментов и линий управления разрядами,
		 * после чего переводит все разряды индикатора в неактивное состояние.
		 */
		void begin() const {
			segment_component::begin();
			digit_component::begin();

			/// Инициализация сигнала в режиме ожидания.
			constexpr auto signal_value = is_common_anode_v<common_mode> ? signal_type::high
				: signal_type::val;
			/// Инициализация всех разрядов дисплея одним значением в режиме ожидания.
			digit_component::write(signal_value);
		}

		/******************************************************************************************
		 * @brief Отображает число на четырёхразрядном индикаторе.
		 *
		 * @param signal_value Значение в диапазоне [0;9999].
		 *                     При превышении диапазона отображается символ "----".
		 */
		FORCEINLINE void show(uint32_t signal_value) {
			update(get_number_map(signal_value)(),
			core::make_index_sequence<digit_component::number_of_pins>{});
		}

	private:
		/******************************************************************************************
		 * @brief Формирует карту отображаемых цифр.
		 *
		 * Разбивает десятичное число на отдельные цифры, каждая из которых соответствует
		 * определённому разряду индикатора.
		 *
		 * Для значений, превышающих максимально отображаемое число (9999),
		 * возвращается карта символов "----".
		 *
		 * @param signal_value Отображаемое значение.
		 *
		 * @return Массив цифр, расположенных в порядке:
		 *         {единицы, десятки, сотни, тысячи}.
		 */
		static constexpr auto get_number_map(uint32_t signal_value) {
			constexpr auto tens = static_cast<uint16_t>(get<1, Te>::value);
			constexpr auto hundreds = static_cast<uint16_t>(get<1, Hu>::value);
			constexpr auto thousands = static_cast<uint16_t>(get<1, Th>::value);
			constexpr uint8_t dash{ 0x0A };

			return signal_value <= 9999 ?
				core::wrapper_for_array<uint32_t, digit_component::number_of_pins>{ signal_value% tens,
				signal_value / tens % tens, signal_value / hundreds % tens, signal_value / thousands % tens } :
				core::wrapper_for_array<uint32_t, digit_component::number_of_pins>{ dash, dash, dash, dash };
		};
		/******************************************************************************************
		 * @brief Обновляет состояние всех разрядов индикатора.
		 *
		 * Последовательно активирует каждый разряд и выводит соответствующую
		 * цифру согласно карте отображения.
		 *
		 * @tparam B  Количество элементов карты.
		 * @tparam Is Индексы разрядов.
		 *
		 * @param number_map Карта отображаемых цифр.
		 * @param core::index_sequence<Is...> Последовательность индексов разрядов.
		 */
		template<uint8_t N, auto... Is>
		void update(const uint32_t(&number_map)[N], core::index_sequence<Is...>) {
			auto write = [this](uint32_t number, uint8_t index) {
				constexpr auto signal_value = is_common_anode_v<common_mode> ? signal_type::val
					: signal_type::high;
				// Записать сигнал в контакты разряда, который нужно обновить
				digit_component::write(signal_value);
				// Вывести значение на сегмент разряда, который нужно обновить
				segment_component::show(static_cast<uint32_t>(_segment_map[number]));
				// Сбросить сигнал на контактах разряда, который нужно обновить
				constexpr auto pins_wrappers = digit_component::get_pins_wrappers();
				digit_component::write(~signal_value, (pins_wrappers())[index]);
			};
			// Вызвать лямбду для каждого разряда, передавая соответствующее значение из number_map
			// и индекс разряда
			(write(number_map[Is], Is), ...);
		}

	private:
		using seg = single_digit_indicator_segments;
		static constexpr seg _segment_map[]{
			/* 0 */ seg::a | seg::b | seg::c | seg::d | seg::e | seg::f,
			/* 1 */ seg::b | seg::c,
			/* 2 */ seg::a | seg::b | seg::g | seg::e | seg::d,
			/* 3 */ seg::a | seg::b | seg::g | seg::c | seg::d,
			/* 4 */ seg::f | seg::g | seg::b | seg::c,
			/* 5 */ seg::a | seg::f | seg::g | seg::c | seg::d,
			/* 6 */ seg::a | seg::f | seg::g | seg::c | seg::d | seg::e,
			/* 7 */ seg::a | seg::b | seg::c,
			/* 8 */ seg::a | seg::b | seg::c | seg::d | seg::e | seg::f | seg::g,
			/* 9 */ seg::a | seg::b | seg::c | seg::d | seg::f | seg::g,
			/* F */ seg::a | seg::f | seg::g | seg::e
		};
	};

} /// !namespace uno

#endif // !FOUR_DIGIT_INDICATOR_H_FOR_THE_ARDUINO_UNO_PROJECT

