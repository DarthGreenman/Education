// my_utilities.h

#ifndef MY_UTILITIES_H
#define MY_UTILITIES_H

#include "my_types.h"

#include <bitset>
#include <stdexcept>
#include <type_traits>

namespace my 
{	
	namespace bit
	{
		namespace helper
		{
			template<std::size_t N>
			auto to_numeric_impl(const std::bitset<N>& bit, std::size_t pos = 0ull, int numeric = 0)
			{
				if (pos == properties_numeric<N>::width)
					return numeric;
				const decltype(numeric) num{ bit.test(pos) << pos };
				return to_numeric_impl(bit, ++pos, numeric += num);
			}			
		}

		template<std::size_t N>
		auto copy(const std::bitset<N>& sourсe, std::bitset<N>& target)
		{
			// Оператор | осуществляет операцию логического ИЛИ между соответствующими битами двух операндов.
			// Результатом операции логического ИЛИ будет 0 только в случае, если оба бита равны 0.
			// Во всех остальных случаях результат будет 1.
			target |= sourсe;
			// Оператор & осуществляет операцию логического И между соответствующими битами двух операндов.
			// Результатом операции логического И будет 1 только в случае, если оба бита равны 1.
			// Во всех других случаях результат будет 0.
			target &= sourсe;
		}

		// В любой позиционной системе счисления операции сложения и вычитания двух чисел осуществляются поразрядно,
		// начиная с младших разрядов. При сложении переполнение из младшего разряда переносится в старший разряд.
		// Сумма каждого i-ого разряда получается в результате сложения ai + bi + 1, где 1 соответствует переносу из 
		// младшего разряда в текущий. Перенос происходит в том случае, если в младшем разряде сумма больше или равна
		// основанию системы счисления.
		template<std::size_t N>
		auto add(const std::bitset<N>& lhs, const std::bitset<N>& rhs) -> std::decay_t<decltype(lhs)>
		{
			const auto carry = (lhs & rhs) << 1;
			// Оператор ^ осуществляет операцию логического исключающего ИЛИ между соответствующими битами
			// двух операндов. Результатом операции логического исключающего ИЛИ будет 0 в случае равенства битов.
			// Во всех остальных случаях результат будет 1.
			const auto sum = lhs ^ rhs;
			return carry == 0 ? sum : add(sum, carry);
		}


		//   0000 1001 0010 1001  (929)
		// + 0001 0101 0011 1000  (1538)
		//   ___________________
		//   0001 1110 0110 0001  (7777) - двоичная сумма
		// + 0110      0110  поправки(по правилу 1 и правилу 2)
		//   ___________________
		//   0010 0100 0110 0111  (2467) - сумма BCD
		
		// Правило 1 - к тетраде из которой был перенос нужно прибавить 0110.
		// Правило 2 - к тетраде, которая больше 1001 нужно прибавить 0110.
		template<std::size_t N>
		auto adjusted(const std::bitset<N>& number, std::size_t offset = 0ull)
		{
			if (offset == N)
				return number;

			using properties_numeric = bit::properties_numeric<N>;
			const auto mask = properties_numeric::lsb ^ properties_numeric::msb;
			const decltype(number) numeric{ number >> offset & mask };
			if (offset == N - properties_numeric::width && properties_numeric::is_adjust(numeric))
				throw std::overflow_error{ "The calculation result is too large for the target type." };
			const auto adj = properties_numeric::adj << offset;

			return adjusted(properties_numeric::is_adjust(numeric) ? bit::add(number, adj) : number,
				offset += properties_numeric::width);
		}

		template<std::size_t N>
		auto add_bcd(const std::bitset<N>& lhs, const std::bitset<N>& rhs)
		{
			const auto sum = bit::add(lhs, rhs);
			return bit::adjusted(sum);
		}
		
		template<std::size_t N>
		auto to_numeric(const std::bitset<N>& bit)
		{
			return helper::to_numeric_impl(bit);
		}
	}
}

#endif // !MY_UTILITIES_H