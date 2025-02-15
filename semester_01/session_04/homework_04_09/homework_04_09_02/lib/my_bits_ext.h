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
			// ������ ���� �� ������� � �������. �� ������ ���� ������ �����
			// ���������� ��������� ���������.
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
		void copy(const std::bitset<N>& Sour�e, std::bitset<N>& Target) {
			// �������� | ������������ �������� ����������� ��� ����� ���������������� ������ ���� ���������.
			// ����������� �������� ����������� ��� ����� 0 ������ � ������, ���� ��� ���� ����� 0.
			// �� ���� ��������� ������� ��������� ����� 1.
			Target |= Sour�e;
			// �������� & ������������ �������� ����������� � ����� ���������������� ������ ���� ���������.
			// ����������� �������� ����������� � ����� 1 ������ � ������, ���� ��� ���� ����� 1.
			// �� ���� ������ ������� ��������� ����� 0.
			Target &= Sour�e;
		}

		// � ����� ����������� ������� ��������� �������� �������� � ��������� ���� ����� �������������� ����������,
		// ������� � ������� ��������. ��� �������� ������������ �� �������� ������� ����������� � ������� ������.
		// ����� ������� i-��� ������� ���������� � ���������� �������� ai + bi + 1, ��� 1 ������������� �������� �� 
		// �������� ������� � �������. ������� ���������� � ��� ������, ���� � ������� ������� ����� ������ ��� �����
		// ��������� ������� ���������.
		template<std::size_t N>
		auto add(const std::bitset<N>& Lhs, const std::bitset<N>& Rhs) -> std::decay_t<decltype(Lhs)> {
			const auto carry = (Lhs & Rhs) << 1;
			// �������� ^ ������������ �������� ����������� ������������ ��� ����� ���������������� ������
			// ���� ���������. ����������� �������� ����������� ������������ ��� ����� 0 � ������ ��������� �����.
			// �� ���� ��������� ������� ��������� ����� 1.
			const auto sum = Lhs ^ Rhs;
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

		// ���������� ��������, �� 0 �� 9, �� �������� ���������.
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