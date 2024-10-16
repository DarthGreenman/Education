// my_utilities.h

#ifndef MY_UTILITIES_H
#define MY_UTILITIES_H

#include <bitset>
#include <cstddef>
#include <type_traits>

namespace my 
{
	namespace bits
	{
		namespace helper
		{
			template<std::size_t N>
			auto to_numeric_impl(const std::bitset<N>& bits, std::size_t pos = 0ull, int numeric = 0) {
				if (pos == 8ull)
					return numeric;
				const decltype(numeric) num{ bits.test(pos) << pos };
				return to_numeric_impl(bits, ++pos, numeric += num);
			}
		}

		template<std::size_t N>
		void copy(const std::bitset<N>& sour�e, std::bitset<N>& target) {
			// �������� | ������������ �������� ����������� ��� ����� ���������������� ������ ���� ���������.
			// ����������� �������� ����������� ��� ����� 0 ������ � ������, ���� ��� ���� ����� 0.
			// �� ���� ��������� ������� ��������� ����� 1.
			target |= sour�e;
			// �������� & ������������ �������� ����������� � ����� ���������������� ������ ���� ���������.
			// ����������� �������� ����������� � ����� 1 ������ � ������, ���� ��� ���� ����� 1.
			// �� ���� ������ ������� ��������� ����� 0.
			target &= sour�e;
		}

		// � ����� ����������� ������� ��������� �������� �������� � ��������� ���� ����� �������������� ����������,
		// ������� � ������� ��������. ��� �������� ������������ �� �������� ������� ����������� � ������� ������.
		// ����� ������� i-��� ������� ���������� � ���������� �������� ai + bi + 1, ��� 1 ������������� �������� �� 
		// �������� ������� � �������. ������� ���������� � ��� ������, ���� � ������� ������� ����� ������ ��� �����
		// ��������� ������� ���������.
		template<std::size_t N>
		auto add(const std::bitset<N>& lhs, const std::bitset<N>& rhs) -> std::decay_t<decltype(lhs)> 
		{
			const auto carry = (lhs & rhs) << 1;
			// �������� ^ ������������ �������� ����������� ������������ ��� ����� ���������������� ������
			// ���� ���������. ����������� �������� ����������� ������������ ��� ����� 0 � ������ ��������� �����.
			// �� ���� ��������� ������� ��������� ����� 1.
			const auto sum = lhs ^ rhs;
			return carry == 0 ? sum : add(sum, carry);
		}

		/*
		   0000 1001 0010 1001  (929)
		 + 0001 0101 0011 1000  (1538)
		   ___________________
		   0001 1110 0110 0001  (7777) - �������� �����
		 +      0110      0110  �������� (�� ������� 1 � ������� 2)
		   ___________________
		   0010 0100 0110 0111  (2467) - ����� BCD
		*/
		// ������� 1 - � ������� �� ������� ��� ������� ����� ��������� 0110.
		// ������� 2 - � �������, ������� ������ 1001 ����� ��������� 0110.
		template<std::size_t N>
		auto adj(const std::bitset<N>& number) 
		{
			constexpr std::decay_t<decltype(number)> six{ 0b0110 };
			auto adjusted_number = add(number, six);
			if (constexpr decltype(adjusted_number) mask{ 0b1111 }; (adjusted_number & ~mask) != 0)
			{
				adjusted_number &= mask;
				adjusted_number |= (1 << 8ull);
			}
			return adjusted_number;
		}

		template<std::size_t N>
		auto to_numeric(const std::bitset<N>& bits) {
			return helper::to_numeric_impl(bits);
		}
	}
}

#endif // !MY_UTILITIES_H