// ultralong.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "includes/my_ultralong.h"

#include <exception>
#include <iostream>
#include <vector>

int main()
{
	try
	{
		const my::ultralong<64> number_1{ std::vector<char>{'2','4','5','6','4','5','3'} };
		const my::ultralong<64> number_2{ "2456453" };
		const my::ultralong<64> number_3{ std::vector<unsigned>{2,4,5,6,4,5,3} };
		const my::ultralong<64> number_4{ {2,4,5,6,4,5,3} };


		std::cout << number_1 << '\n';
		std::cout << number_2 << '\n';
		std::cout << number_3 << '\n';
		std::cout << number_4 << '\n';
	}
	catch (const std::exception& err)
	{
		std::cout << err.what();
	}	
	return 0;
}