/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * brief Функции помошники на этапе компиляции.
 *
 */

#ifndef ADDONS_H_FOR_THE_ARDUINO_UNO_PROJECT
#define ADDONS_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <meta.h>

#if defined(_MSC_VER)
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <stdint.h>
#endif

namespace uno {
	/**
	 * @brief
	 * @param value
	 * @return
	 */
	constexpr unsigned long long int operator"" _kOhm(unsigned long long int value) noexcept {
		return value * 1000ULL;
	}

	constexpr unsigned long long int operator"" _sec(unsigned long long int value) noexcept {
		return value * 1000ULL;
	}

	constexpr unsigned long long int operator"" _ms(unsigned long long int value) noexcept {
		return value;
	}


	/**
	 * @brief Структура для "привязки" пина и сущности.
	 *
	 * Эта структура используется для связывания конкретного пина с определенной
	 * сущностью, все сущности должны быть уникальными для каждого пина. Это
	 * позволяет легко управлять различными устройствами или компонентами,
	 * подключенными к плате Arduino Uno, через связанные пины.
	 *
	 * @attention Важно использовать только перечисленные ниже сущности, чтобы
	 *            избежать конфликтов и обеспечить корректную работу системы.
	 *
	 * @tparam Pin    пин, который может быть использован для управления
	 *                различными компонентами или устройствами
	 * @tparam Entity сущность, которая "привязывается" к пину. Это может быть
	 *                идентификатор цвета, состояния или любой другой параметр,
	 *                который необходимо связать с пином.
	 *
	 * Список допустимых сущностей, значения опущены:
	 *      - enum class core::color;
	 *      - enum class joystick_controls;
	 *      - enum class digit_of_the_number;
	 *      - enum class registers;
	 *      - enum class membrane_keypad_channels;
	 *      - enum class single_digit_indicator_segments;
	 *      - enum class hc_sr04_channels
	 */
	template <uint8_t Pin, auto Entity = 0>
	struct bind {
		static constexpr auto pin = Pin;
		static constexpr auto entity = Entity;
	};

	/**
	 * @brief Структура для получения привязки по индексу.
	 *
	 * @tparam N    индекс, по которому нужно получить привязку, используется
	 *              для доступа к элементам в списке привязок
	 * @tparam Bind тип привязки, из которой нужно извлечь значение. Это может
	 *              быть любой тип, который содержит статические члены pin и entity
	 */
	template <uint8_t N, typename Bind> struct get;
	template <typename Bind> struct get<0, Bind> { static constexpr auto value = Bind::pin; };
	template <typename Bind> struct get<1, Bind> { static constexpr auto value = Bind::entity; };


	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		template<typename B, auto... Args>
		struct is_valid_bindings { static constexpr bool value = ((get<1, B>::value == Args) || ...); };
	} /// !namespace helper
	/// @endcond

	/**
	 * @brief Содержит результат проверки, является ли "привязка" допустимой.
	 * @tparam B       "привязка", которую нужно проверить на соответствие одной
	 *                 из списка допустимых
	 * @tparam ...Args список допустимых "привязок"
	 */
	template<typename B, auto... Args>
	inline constexpr bool is_valid_bindings_v = helper::is_valid_bindings<B, Args...>::value;

	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		/*
		 * Поиск привязки, соответсвующей сущности E, выполняется 
		 * последовательно по списку типов. Возвращается первый тип, для 
		 * которого существует привязка к сущности E.
		 * Если подходящий тип отсутствует, результатом поиска является void.
		 * @tparam E  сущность
		 * @tparam Ts список типов
		 */
		template<auto E, typename... Ts>
		struct find_bind;

		/*
		 * Рекурсивный шаг поиска. Если Head привязан к E, поиск завершается и
		 * результатом становится Head. В противном случае поиск продолжается
		 * в оставшемся списке Tail.
		 * @tparam E    сущность
		 * @tparam Head текущий тип списка типов
		 * @tparam Tail типы списка расположенные за текущим
		 */
		template<auto E, typename Head, typename... Tail>
		struct find_bind<E, Head, Tail...> {
			using type =
				core::conditional_t<(uno::get<1, Head>::value == E),
				Head,
				typename find_bind<E, Tail...>::type>;
		};

		/*
		 * Терминальный случай рекурсии. Пустой список означает, что все
		 * кандидаты проверены, но привязка не найдена.
		 */
		template<auto E>
		struct find_bind<E> { using type = void; };
	} /// !namespace helper
	/// @endcond
	
	template<auto E, typename... Args>
	using find_bind_t = typename helper::find_bind<E, Args...>::type;


	/// @cond HIDE_ENUM_BRIEF
	/// Предварительное объявление перечислений событий
	namespace event {
		enum class button : uint8_t;
		enum class resistance : uint8_t;
		enum class movement : uint8_t;
		enum class key : uint8_t;
	} /// !namespace event

	template<typename E>
	struct enum_traits;

	template<>
	struct enum_traits<event::button> {
		using type = uint8_t;
		static constexpr uint8_t size{ 2 };
	};

	template<>
	struct enum_traits<event::resistance> {
		using type = uint8_t;
		static constexpr uint8_t size{ 3 };
	};

	template<>
	struct enum_traits<event::movement> {
		using type = uint8_t;
		static constexpr uint8_t size{ 5 };
	};

	template<>
	struct enum_traits<event::key> {
		using type = uint8_t;
		static constexpr uint8_t size{ 3 };
	};
	/// @endcond
} /// !namespace uno

#endif /// !defined(ADDONS_H_FOR_THE_ARDUINO_UNO_PROJECT)

