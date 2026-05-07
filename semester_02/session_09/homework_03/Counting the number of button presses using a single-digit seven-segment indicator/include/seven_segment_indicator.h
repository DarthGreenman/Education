// seven_segment_indicator.h

#ifndef SEVEN_SEGMENT_INDICATOR_H_FOR_THE_ARDUINO_UNO_PROJECT
#define SEVEN_SEGMENT_INDICATOR_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "hardware.h" 
#include "signal.h"
#include "types_used.h"

namespace uno {
	template <uno::common_pin Com = uno::common_pin::anode>
	class seven_segment_indicator {
	public:
		constexpr static uint8_t number_of_channels{ 8 };
		constexpr seven_segment_indicator() = default;
		/**
		 * @brief Конструирует объект, используя свойство вариадического шаблона раскрываться в последовательность аргументов
		 * @tparam ...B Пакет параметров элементов однородного типа
		 * @param ...binds Аргумент содержит пару связанных значений, в данном случае номер контакта на плате и идентификатор
		 * сегмента одноразрядного семисегментного индикатора
		 */
		template <typename... B> explicit seven_segment_indicator(B&&... binds) {
			static_assert(fox::package_size<B...>::value == number_of_channels || fox::package_size<B...>::value == number_of_channels - 1,
				"The number of contacts is incorrect, it should be seven or eight.");

			auto construct = [this](auto&& bind) {
				using namespace uno;
				_component.set(bind.first, pinout(bind.second));
				_component.write(Com == common_pin::anode ? signal::digital::high : signal::digital::low, pinout(bind.second));
			};
			(construct(binds), ...);
		}

		seven_segment_indicator(const seven_segment_indicator&) = delete;
		seven_segment_indicator(seven_segment_indicator&&) = delete;
		~seven_segment_indicator() = default;

		seven_segment_indicator& operator=(const seven_segment_indicator&) = delete;
		seven_segment_indicator& operator=(seven_segment_indicator&&) = delete;

		/** 
		 * @brief Выводит на индикатор заданные сегменты
		 * @param flags Битовая маска сегментов для отображения
		 */
		void show(uno::single_seven_segment_indicator flags) {
			using namespace uno;
			for(auto id = single_seven_segment_indicator::a; id != single_seven_segment_indicator::dp; ++id) {
				const auto signal_value = uno::has(flags, id) & (Com == common_pin::anode) ? signal::digital::low : signal::digital::high;
				_component.write(signal_value, pinout(id));
			}
		}

	private:
		/**
		 * @brief Возвращает порядковый номер сегмента в компоненте
		 * @param value Идентификатор сегмента индикатора
		 * @return Если идентификатор определен то возвращает порядковый номер от 0 до 7 вкл., иначе - 255
		 */
		constexpr static decltype(auto) pinout(uno::single_seven_segment_indicator bind_param) {
			using namespace uno;
			return static_cast<uint8_t>(
				has(bind_param, single_seven_segment_indicator::a) ? 0 :
				has(bind_param, single_seven_segment_indicator::b) ? 1 :
				has(bind_param, single_seven_segment_indicator::c) ? 2 :
				has(bind_param, single_seven_segment_indicator::d) ? 3 :
				has(bind_param, single_seven_segment_indicator::e) ? 4 :
				has(bind_param, single_seven_segment_indicator::f) ? 5 :
				has(bind_param, single_seven_segment_indicator::g) ? 6 :
				has(bind_param, single_seven_segment_indicator::dp) ? 7u :
				-1
				);
		}

		uno::electronic_component<uno::signal::digital, uno::signal::output, number_of_channels> _component{};
	};

} /// namespace uno

#endif // !SEVEN_SEGMENT_INDICATOR_H_FOR_THE_ARDUINO_UNO_PROJECT