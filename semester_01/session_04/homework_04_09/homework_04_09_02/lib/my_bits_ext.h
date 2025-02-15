// my_bits_ext.h

#ifndef MY_BITS_EXT_H
#define MY_BITS_EXT_H

#include "my_bcd.h"
#include <bitset>
#include <cstddef>
#include <limits>
#include <type_traits>

namespace numbers {
	namespace bits_ext {
		namespace helper {
			// Читает биты от старших к младшим. Из первой пары разных битов
			// возвращает результат сравнения.
			template<std::size_t N>
			auto greater(const std::bitset<N>& Lhs, const std::bitset<N>& Rhs) {
				for (auto pos = N - 1_z; pos != end_reverse_v<decltype(pos)>; --pos)
					if (const auto is_different_bits = Lhs.test(pos) ^ Rhs.test(pos); is_different_bits != 0)
						return (Lhs.test(pos) & 1) > (Rhs.test(pos) & 1);
				return false;
			}
			
			template<std::size_t N>
			auto to_numeric_impl(const std::bitset<N>& Bits, std::size_t Pos = 0_z, int Numeric = 0) {
				if (Pos == bcd<packed>::width_unit)
					return Numeric;
				const decltype(Numeric) numeric{ Bits.test(Pos) << Pos };
				return to_numeric_impl(Bits, ++Pos, Numeric += numeric);
			}
		}
		
		template<std::size_t N>
		void copy(const std::bitset<N>& Sourсe, std::bitset<N>& Target) {
			// Оператор | осуществляет операцию логического ИЛИ между соответствующими битами двух операндов.
			// Результатом операции логического ИЛИ будет 0 только в случае, если оба бита равны 0.
			// Во всех остальных случаях результат будет 1.
			Target |= Sourсe;
			// Оператор & осуществляет операцию логического И между соответствующими битами двух операндов.
			// Результатом операции логического И будет 1 только в случае, если оба бита равны 1.
			// Во всех других случаях результат будет 0.
			Target &= Sourсe;
		}

		// В любой позиционной системе счисления операции сложения и вычитания двух чисел осуществляются поразрядно,
		// начиная с младших разрядов. При сложении переполнение из младшего разряда переносится в старший разряд.
		// Сумма каждого i-ого разряда получается в результате сложения ai + bi + 1, где 1 соответствует переносу из 
		// младшего разряда в текущий. Перенос происходит в том случае, если в младшем разряде сумма больше или равна
		// основанию системы счисления.
		template<std::size_t N>
		auto add(const std::bitset<N>& Lhs, const std::bitset<N>& Rhs) -> std::decay_t<decltype(Lhs)> {
			const auto carry = (Lhs & Rhs) << 1;
			// Оператор ^ осуществляет операцию логического исключающего ИЛИ между соответствующими битами
			// двух операндов. Результатом операции логического исключающего ИЛИ будет 0 в случае равенства битов.
			// Во всех остальных случаях результат будет 1.
			const auto sum = Lhs ^ Rhs;
			return carry == 0 ? sum : add(sum, carry);
		}

		/*
		   0000 1001 0010 1001  (929)
		 + 0001 0101 0011 1000  (1538)
		   ___________________
		   0001 1110 0110 0001  (7777) - двоичная сумма
		 +      0110      0110  поправки (по правилу 1 и правилу 2)
		   ___________________
		   0010 0100 0110 0111  (2467) - сумма BCD
		*/
		// Правило 1 - к тетраде из которой был перенос нужно прибавить 0110.
		// Правило 2 - к тетраде, которая больше 1001 нужно прибавить 0110.
		template<std::size_t N, typename traits>
		auto adj(const std::bitset<N>& Bits) {
			static constexpr std::decay_t<decltype(Bits)> six{ 0b0110 };
			auto bcd = add(Bits, six);
			if (static constexpr decltype(bcd) mask{ 0b1111 };
				is_unpacked_v<traits> && (bcd & ~mask) != 0) {
				bcd &= mask;
				bcd |= (1 << traits::width_unit);
			}
			return bcd;
		}

		// Возвращает значение, от 0 до 9, из младшего полубайта.
		template<std::size_t N>
		auto to_numeric(const std::bitset<N>& Bits) {
			return helper::to_numeric_impl(Bits);
		}

		template<std::size_t N>
		auto operator>(const std::bitset<N>& Lhs, const std::bitset<N>& Rhs) {
			return Lhs == Rhs ? false :
				helper::greater(Lhs, Rhs);
		}

		template<std::size_t N>
		auto operator<(const std::bitset<N>& Lhs, const std::bitset<N>& Rhs) {
			return Lhs == Rhs ? false :
				!helper::greater(Lhs, Rhs);
		}

		template<std::size_t N>
		auto operator+(const std::bitset<N>& Lhs, const std::bitset<N>& Rhs) {
			return add(Lhs, Rhs);
		}
	}
}

#endif // !MY_BITS_EXT_H