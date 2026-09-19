/// common_pin.h

#ifndef COMMON_PIN_H_FOR_THE_ARDUINO_UNO_PROJECT
#define COMMON_PIN_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <type_traits.h>

#if defined(_MSC_VER)
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <stdint.h>
#endif

namespace uno {

	namespace helper {
		/******************************************************************************************
		 * @brief Тип подключения к общему контакту
		 *
		 * anode   Common Anode, для включения компонента необходимо подать низкий уровень
		 *         сигнала (LOW) на его катoд
		 * cathode Common Cathode, для включения сегмента необходимо подать ВЫСОКИЙ уровень
		 *         сигнала (HIGH) на его анод
		 */
		enum class common_pin_value : uint8_t { anode, cathode };
		template<auto N>
		struct common_pin_mode { static constexpr auto value = N; };

	} /// !namespace helper

	/// Предопределённые типы режимов подключения к общему контакту
	using common_anode = helper::common_pin_mode<helper::common_pin_value::anode>;
	using common_cathode = helper::common_pin_mode<helper::common_pin_value::cathode>;

	template <typename T>
	inline constexpr auto is_common_anode_v = core::is_same_v<T, common_anode>;
	template <typename T>
	inline constexpr auto is_common_cathode_v = core::is_same_v<T, common_cathode>;

} /// !namespace uno

#endif /// !defined(COMMON_PIN_H_FOR_THE_ARDUINO_UNO_PROJECT)

