// ultralong.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "includes/my_ultralong.h"
#include "includes/my_ultralong_ext.h"

#include <exception>
#include <iostream>
#include <string>
#include <vector>

int main()
{
	try
	{
		my::ultralong<40> number_1{ std::vector<unsigned>{9,2,9} }; // 9,2,9
		my::ultralong<40> number_2{ U"1538" }; // 1538

		std::cout << number_1 << '\n';
		std::cout << number_2 << '\n';
		
		number_1 += number_2;
		std::cout << number_1 << '\n';

		const auto sum = number_1 + number_2;
		std::cout << sum << '\n';
	
		const my::ultralong<64> number_3{ std::vector<unsigned>{2,0,5,6,4,5,0} };
		const my::ultralong<64> number_4{ {'2','0','5','6','4','5','0'} };
		const my::ultralong<64> number_5{ L"2056450" };
		const my::ultralong<64> number_6{ U"2056450" };
		
		std::cout << number_1 << '\n';
		std::cout << number_2 << '\n';
		std::cout << number_3 << '\n';
		std::cout << number_4 << '\n';
		std::cout << number_5 << '\n';
		std::cout << number_6 << '\n';
		
		
		std::string report_1{ "Balance at the time of reporting: " };
		const auto sum_1 = my::to_string(number_1);
		report_1 += sum_1;
		std::cout << report_1 << '\n';

		std::wstring report_2{ L"Balance at the time of reporting : " };
		const auto sum_2 = my::to_string<std::wstring>(number_2);
		report_2 += sum_2;
		std::wcout << report_2;

		const auto sum_3 = my::to_string<std::u8string>(number_3);
		const auto sum_4 = my::to_string<std::u16string>(number_3);
		const auto sum_5 = my::to_string<std::u32string>(number_3);
	
	}
	catch (const std::exception& err)
	{
		std::cout << err.what();
	}

	return 0;
}