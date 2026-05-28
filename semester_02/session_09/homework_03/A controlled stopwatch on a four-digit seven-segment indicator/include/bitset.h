/// bitset.h

#ifndef BITSET_H_FOR_THE_ARDUINO_UNO_PROJECT
#define BITSET_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <stdint.h>

namespace fox {

	/*************************************************************************************************
	 * @brief Упрощенный аналог std::bitset для представления битовых масок, используемых в проекте. 
	 * Предоставляет базовые операции над битами и позволяет использовать перечисления для удобства.
	 * @tparam U Базовый тип для хранения битовой маски
	 */
	template <typename U = uint32_t>
	class bitset {
	public:
		using Underlying_type = U;
		constexpr bitset() = default;
		constexpr explicit bitset(U value) noexcept : _value{ value } {}
		bitset(const bitset&) = default;
		bitset(bitset&&) = default;
		~bitset() = default;

		bitset& operator=(const bitset&) = default;
		bitset& operator=(bitset&&) = default;

		constexpr U get() const noexcept { return _value; }
		constexpr explicit operator U() const noexcept { return _value; }
		
		constexpr decltype(auto) operator | (const bitset& other) const noexcept { return bitset{ _value | other._value }; }
		constexpr decltype(auto) operator & (const bitset& other) const noexcept { return bitset{ _value & other._value }; }
		constexpr decltype(auto) operator ^ (const bitset& other) const noexcept { return bitset{ _value ^ other._value }; }
		constexpr decltype(auto) operator ~ () const noexcept { return bitset{ ~_value }; }
		
		constexpr bool operator != (const bitset& other) const noexcept { return _value != other._value; }
		constexpr bool operator == (const bitset& other) const noexcept { return _value == other._value; }

	private:
		Underlying_type _value{};
	};
	/**********************************************************************************************
	 * @brief Проверяет наличие бит
	 * @tparam E Перечисление констант
	 * @tparam U Базовый тип перечисления
	 * @tparam G Количество бит для представления одного значения
	 * @param in_this Где проверить
	 * @param it Что проверить
	 * @return
	 */
	template <typename U>
	constexpr bool has(const bitset<U>& in_this, U it) noexcept { return (in_this.get() & it) != 0; }

	/**********************************************************************************************
	 * @brief Выполняет декомпозицию
	 * @tparam T Тип элементов декомпозиции
	 * @tparam ...N Список бит по которым необходимо выполнить декомпозицию
	 * @param value Объект декомпозиции
	 * @param mask Маска
	 * @return
	 */
	template <typename T, uint8_t... N>
	constexpr inline decltype(auto) decompose(uint32_t value, uint8_t mask) {
		static_assert((!(N % 4) && ...), "All bit offsets N must be multiples of 4.");
		static_assert(((N < sizeof(uint32_t) * 8) && ...), "Each N must be < number of bits in T.");

		auto decomposing = [&](uint32_t n) { return static_cast<T>((value >> n) & mask); };
		return uno::wrapper_for_array<T, sizeof...(N)>{ decomposing(N)... };
	}

} /// !namespace fox

#endif /// !BITSET_H_FOR_THE_ARDUINO_UNO_PROJECT
