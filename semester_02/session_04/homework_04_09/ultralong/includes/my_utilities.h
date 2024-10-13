// my_utilities.h

#ifndef MY_UTILITIES_H
#define MY_UTILITIES_H

#include <bitset>
#include <cstddef>

namespace my 
{
	namespace bits
	{
		template<std::size_t N>
		void copy(const std::bitset<N>& sourсe, std::bitset<N>& target) {
			// Оператор | осуществляет операцию логического ИЛИ между соответствующими битами двух операндов.
			// Результатом операции логического ИЛИ будет 0 только в случае, если оба бита равны 0.
			// Во всех остальных случаях результат будет 1.
			target |= sourсe;
			// Оператор & осуществляет операцию логического И между соответствующими битами двух операндов.
			// Результатом операции логического И будет 1 только в случае, если оба бита равны 1.
			// Во всех других случаях результат будет 0.
			target &= sourсe;
		}
	}
}

#endif // !MY_UTILITIES_H