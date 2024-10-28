// my_utilities.h

#ifndef MY_UTILITIES_H
#define MY_UTILITIES_H

#include "my_types.h"

#include <bitset>
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
		
		template<std::size_t N>
		auto to_numeric(const std::bitset<N>& bit)
		{
			return helper::to_numeric_impl(bit);
		}
	}
}

#endif // !MY_UTILITIES_H