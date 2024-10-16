// ultralong.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "includes/my_types.h"
#include "includes/my_ultralong.h"
#include "includes/my_utilities.h"

#include <bitset>
#include <exception>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

int main()
{
	try
	{
		/*
		const my::ultralong<64> number_1{ std::vector<char32_t>{'2','0','5','6','4','5','0'} };
		const my::ultralong<64> number_2{ "2056450" };
		const my::ultralong<64> number_3{ std::vector<unsigned>{2,0,5,6,4,5,0} };
		const my::ultralong<64> number_4{ {'2','0','5','6','4','5','0'} };
		const my::ultralong<64> number_5{ L"2056450" };
		const my::ultralong<64> number_6{ U"2056450" };
		
		std::cout << number_1 << '\t' << number_1.to_string() << '\n';
		std::cout << number_2 << '\t' << number_2.to_string() << '\n';
		std::cout << number_3 << '\t' << number_3.to_string() << '\n';
		std::cout << number_4 << '\t' << number_4.to_string() << '\n';
		std::cout << number_5 << '\t' << number_5.to_string() << '\n';
		std::cout << number_6 << '\t' << number_6.to_string() << '\n';
		*/
		/*
		std::string report_1{ "Сальдо на момент отчета: " };
		const auto sum_1 = number_1.to_string();
		report_1 += sum_1;

		std::wstring report_2{ L"Сальдо на момент отчета: " };
		const auto sum_2 = number_2.to_string<std::wstring>();
		report_2 += sum_2;

		const auto sum_3 = number_3.to_string<std::u8string>();
		const auto sum_4 = number_3.to_string<std::u16string>();
		const auto sum_5 = number_3.to_string<std::u32string>();
		*/
	}
	catch (const std::exception& err)
	{
		std::cout << err.what();
	}

	std::bitset<8> a{ 5 };
	std::bitset<8> b{ 7 };
	b.flip();
	const auto c = my::bits::add(b, std::bitset<8>{1});

	const auto sum_b = my::bits::add(a, c);
	const auto sum_d = sum_b.to_ulong();

	my::ultralong<64> number_21{ "2056450" };
	const decltype(number_21) number_22{ number_21 };

	//std::cout << number_21 << '\t' << number_21.to_string() << '\n';
	//std::cout << number_22 << '\t' << number_22.to_string() << '\n';

	decltype(number_21) number_23{ U"123" };

	my::swap(number_23, number_21);
	//std::cout << number_21 << '\t' << number_21.to_string() << '\n';

	std::cout << number_23 << '\t' << number_23.to_string() << '\n';
	decltype(number_21) number_24{ std::move(number_23) };
	std::cout << number_23 << '\t' << number_23.to_string() << '\n';
	std::cout << number_24 << '\t' << number_24.to_string() << '\n';


	return 0;
}