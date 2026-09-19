/// addons.h

#ifndef ADDONS_H_FOR_THE_ARDUINO_UNO_PROJECT
#define ADDONS_H_FOR_THE_ARDUINO_UNO_PROJECT

#if defined(_MSC_VER)
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <stdint.h>
#endif

namespace uno {

	constexpr unsigned long long int operator"" _kOhm(unsigned long long int value) noexcept {
		return value * 1000ULL;
	}


	/**********************************************************************************************
	 * @brief Структура для привязки пина и сущности.
	 *
	 * Эта структура используется для связывания конкретного пина с определенной сущностью, все
	 * сущности должны быть уникальными для каждого пина. Это позволяет легко управлять различными
	 * устройствами или компонентами, подключенными к плате Arduino Uno, через связанные пины.
	 *
	 * @attention     Важно использовать только перечисленные ниже сущности, чтобы избежать конфликтов
	 *                и обеспечить корректную работу системы.
	 *
	 * @tparam Pin    Пин, который может быть использован для управления различными компонентами
	 *                или устройствами.
	 * @tparam Entity Сущность, которая привязывается к пину. Это может быть идентификатор цвета,
	 *                состояния или любой другой параметр, который необходимо связать с пином.
	 *                Список допустимых сущностей, значения опущены:
	 *                     - enum class core::color
	 */
	template <uint8_t Pin, auto Entity = 0>
	struct bind {
		static constexpr auto pin = Pin;
		static constexpr auto entity = Entity;
	};


	/**********************************************************************************************
	 * @brief Структура для получения привязки по индексу.
	 *
	 * @tparam N    Индекс, по которому нужно получить привязку. Обычно используется для доступа к
	 *              элементам в массиве или списке привязок.
	 * @tparam Bind Тип привязки, из которой нужно извлечь значение. Это может быть любой тип, который
	 *              содержит статические члены pin и entity.
	 */
	template <uint8_t N, typename Bind> struct get;
	template <typename Bind> struct get<0, Bind> { static constexpr auto value = Bind::pin; };
	template <typename Bind> struct get<1, Bind> { static constexpr auto value = Bind::entity; };


	namespace helper {

		template<typename B, auto... Args>
		struct is_valid_bindings {
			static constexpr bool value = ((get<1, B>::value == Args) || ...);
		};

	} /// !namespace helper

	template<typename B, auto... Args>
	inline constexpr bool is_valid_bindings_v = helper::is_valid_bindings<B, Args...>::value;


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

} /// !namespace uno

#endif /// !defined(ADDONS_H_FOR_THE_ARDUINO_UNO_PROJECT)

