/// single_digit_indicator.h

#ifndef SEVEN_SEGMENT_INDICATOR_H_FOR_THE_ARDUINO_UNO_PROJECT
#define SEVEN_SEGMENT_INDICATOR_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <addons.h>
#include <algorithms.h>
#include <basic_types.h>
#include <common.h>
#include <common_pin.h>
#include <electronic_component.h>
#include <log.h>
#include <meta.h>

#if defined(_MSC_VER)
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <stdint.h>
#endif

namespace uno {

	/**********************************************************************************************
	 * @brief Перечисление сегментов одноразрядного семисегментного индикатора.
	 *
	 * Каждый элемент перечисления соответствует одному сегменту индикатора. Значения представлены в
	 * виде битовых масок, что позволяет объединять несколько сегментов в одно целочисленное значение
	 * для формирования отображаемого символа.
	 */
	enum class single_digit_indicator_segments : uint8_t {
		a = 0b1000'0000, b = 0b0100'0000, c = 0b0010'0000, d = 0b0001'0000, e = 0b0000'1000,
		f = 0b0000'0100, g = 0b0000'0010, dp = 0b0000'0001
	};

	/**********************************************************************************************
	 * Операторы для перечислений, которые позволяют использовать их значения в битовых операциях
	 * (|, ^, &, ~) и сравнениях (==, !=) без необходимости явного приведения типов. */
	using segment = single_digit_indicator_segments;

	FORCEINLINE constexpr segment operator|(segment lhs, segment rhs) noexcept {
		return static_cast<segment>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
	}

	FORCEINLINE constexpr segment operator^(segment lhs, segment rhs) noexcept {
		return static_cast<segment>(static_cast<uint32_t>(lhs) ^ static_cast<uint32_t>(rhs));
	}

	FORCEINLINE constexpr segment operator&(segment lhs, segment rhs) noexcept {
		return static_cast<segment>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
	}

	/**********************************************************************************************
	 * @brief Управляет одноразрядным семисегментным индикатором.
	 *
	 * Класс предоставляет интерфейс для управления состоянием сегментов семисегментного индикатора
	 * и отображения чисел или символов.
	 *
	 * На этапе компиляции выполняется проверка:
	 * - количества подключённых сегментов;
	 * - корректности привязки каждого вывода к соответствующему сегменту.
	 *
	 * Благодаря этому ошибки конфигурации обнаруживаются до загрузки программы в микроконтроллер.
	 *
	 * @tparam C  Тип общего вывода индикатора (общий анод или общий катод).
	 * @tparam Bs Привязки выводов микроконтроллера к сегментам индикатора.
	 */
	template <
		typename C,
		typename... Bs
	>
	class single_digit_indicator : public electronic_component<
		digital_signal,
		output_signal,
		get<0, Bs>::value...
	> {
		static_assert(sizeof...(Bs) == 7 || sizeof...(Bs) == 8, "Incorrect number of pins, "
			"should be seven or eight.");
		/// Проверяем уникальность привязок.
		static_assert(core::is_unique_types_v<Bs...>, "The bindings are not unique..");
		/// Проверяем корректность привязок сегментов a, b, c, d, e, f, g и dp.
		using seg = uno::single_digit_indicator_segments;
		static_assert((is_valid_bindings_v<Bs,
			seg::a, seg::b, seg::c, seg::d, seg::e, seg::f, seg::g, seg::dp> && ...),
			"Wrong binding.");

	public:
		using basic_component = electronic_component<
			digital_signal,
			output_signal,
			get<0, Bs>::value...
		>;

		constexpr single_digit_indicator() = default;
		single_digit_indicator(const single_digit_indicator&) = delete;
		single_digit_indicator(single_digit_indicator&&) = delete;
		~single_digit_indicator() = default;

		single_digit_indicator& operator=(const single_digit_indicator&) = delete;
		single_digit_indicator& operator=(single_digit_indicator&&) = delete;

		/******************************************************************************************
		 * @brief Инициализирует индикатор.
		 *
		 * Настраивает все подключённые выводы как выходы и переводит индикатор в исходное
		 * состояние, при котором все сегменты выключены.
		 */
		void begin() const {
			basic_component::begin();
			write(~encoded_high_signal);
		}

		/******************************************************************************************
		 * @brief Отображает символ на индикаторе.
		 *
		 * @param signal_value Упакованное битовое представление состояния сегментов.
		 *                     Обычно формируется таблицей кодов символов
		 *                     (0–9, A–F и другие).
		 */
		FORCEINLINE void show(uint32_t signal_value) const { write(signal_value); }

	private:
		/******************************************************************************************
		 * @brief Передаёт состояние сегментов на выводы индикатора.
		 *
		 * Выполняет преобразование упакованного значения сегментов в набор
		 * логических уровней, соответствующих подключённым выводам,
		 * после чего записывает их в аппаратные линии.
		 *
		 * @param signal_value Упакованное состояние сегментов.
		 */
		FORCEINLINE void write(uint32_t signal_value) const {
			const auto encoded_signal = is_common_anode_v<C>
				? signal_value ^ encoded_high_signal : signal_value;

			const auto signal_value_map =
				math::decompose<typename basic_component::signal_type::value_type,
				math::lb<static_cast<uint32_t>(get<1, Bs>::value)>...>
				(encoded_signal, 0x1);
			basic_component::write(signal_value_map());
		}

	private:
		/******************************************************************************************
		 * @brief Код неактивного состояния всех сегментов.
		 *
		 * Используется при инициализации индикатора и преобразовании сигналов
		 * для индикаторов с общим анодом или общим катодом.
		 */
		static constexpr auto encoded_high_signal = static_cast<uint32_t>(
			seg::a | seg::b | seg::c | seg::d | seg::e | seg::f | seg::g | seg::dp
			);
	};

} // namespace uno

#endif /// !defined(SEVEN_SEGMENT_INDICATOR_H_FOR_THE_ARDUINO_UNO_PROJECT)
