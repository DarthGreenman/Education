// ultralong.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "includes/my_ultralong.h"
#include "includes/my_utilities.h"

#include <exception>
#include <iostream>
#include <string>
#include <vector>

int main()
{
	try
	{
		
		my::ultralong<40> number_1{ std::vector<unsigned>{9,9,9,9} }; // 9,2,9
		my::ultralong<40> number_2{ U"1" }; // 1538

		std::cout << number_1 << '\t' << number_1.to_string() << '\n';
		std::cout << number_2 << '\t' << number_2.to_string() << '\n';

		number_1 += number_2;

		std::cout << number_1 << '\t' << number_1.to_string() << '\n';

		const auto sum = number_1 + number_2;
		std::cout << sum << '\t' << sum.to_string() << '\n';
	
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
		
		
		std::string report_1{ "Balance at the time of reporting: " };
		const auto sum_1 = number_1.to_string();
		report_1 += sum_1;
		std::cout << report_1 << '\n';

		std::wstring report_2{ L"Balance at the time of reporting: " };
		const auto sum_2 = number_2.to_string<std::wstring>();
		report_2 += sum_2;
		std::wcout << report_2;

		const auto sum_3 = number_3.to_string<std::u8string>();
		const auto sum_4 = number_3.to_string<std::u16string>();
		const auto sum_5 = number_3.to_string<std::u32string>();
	}
	catch (const std::exception& err)
	{
		std::cout << err.what();
	}

	

	return 0;
}