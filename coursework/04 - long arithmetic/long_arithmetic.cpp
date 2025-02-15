// long_arithmetic.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//


#include "includes/my_test.h"
#include "includes/my_ultralong.h"
#include "includes/my_ultralong_ext.h"

#include <exception>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

//#pragma warning(disable:4235)


int main()
{
	try
	{
		using namespace my;
		// Демонстрация работы конструкторов ///////////////////////////////////////////////////////////////////////////////
		constexpr std::size_t w{ 256 };
		const ultralong<w> n1{ "1234567890987654321" };
		const ultralong<w> n2{ L"-1234567890987654321" };
		const ultralong<w> n3{ U"-1234567890987654321" };
		const ultralong<w> n4{ std::vector<unsigned>{1,2,3,4,5,6,7,8,9,0,9,8,7,6,5,4,3,2,1} };
		const ultralong<w> n5{ {'-','1','2','3','4','5','6','7','8','9','0','9','8','7','6','5','4','3','2','1'} };

		// Демонстрация вывода /////////////////////////////////////////////////////////////////////////////////////////////
		auto print = [](const std::string& name, const ultralong<w>& value)
			{
				std::cout << name << " = " << value << '\n';
			};
		print("n1", n1);
		print("n2", n2);
		print("n3", n3);
		print("n4", n4);
		print("n5", n5);

		const ultralong<w> n6{ "1234567890987654321" };
		const ultralong<w> n7{ "-9234567890987654321" };
		print(to_string(n6) + " + " + to_string(n7), n6 + n7);


		// Демонстрация сложения и умножения ///////////////////////////////////////////////////////////////////////////////
		std::mt19937 engine;
		//test(engine, 20, sum<std::size_t>{}, sum<my::ultralong<1024>>{}, '+');		
		//test(engine, 20, sub<std::size_t>{}, sub<my::ultralong<1024>>{}, '-');
		//test(engine, 20, mul<std::size_t>{}, mul<my::ultralong<1024>>{}, '*');

		// Демонстрация инкремента и декеремента ///////////////////////////////////////////////////////////////////////////		
		ultralong<w> n8{ "99" };
		for (auto i = 0ull; i < 5; ++i)
			std::cout << n8++ << '\t';
		
		for (auto i = 0ull; i < 5; ++i)
			std::cout << ++n8 << '\t';

		for (auto i = 0ull; i < 5; ++i)
			std::cout << --n8 << '\t';

		for (auto i = 0ull; i < 5; ++i)
			std::cout << n8-- << '\t';

		std::cout << '\n';
		
		// Демонстрация вычитания //////////////////////////////////////////////////////////////////////////////////////////
		const ultralong<w> n9{ "-199" };
		const ultralong<w> n10{ "-99" };
		print(to_string(n9) + " - " + to_string(n10) + " + " + to_string(n8), n9 - n10 + n8);
		print(to_string(n8) + " + " + to_string(n9) + " - " + to_string(n10), n8 + n9 - n10);

		
		// Демонстрация конвертирования ////////////////////////////////////////////////////////////////////////////////////
		//const auto s1 = to_string(n1);
		//const auto s2 = to_string<std::wstring>(n2);
		//const auto s3 = to_string<std::u8string>(n3);
		//const auto s4 = to_string<std::u16string>(n4);
		//const auto s5 = to_string<std::u32string>(n5);

		// Демонстрация обнаружения ошибок /////////////////////////////////////////////////////////////////////////////////
		// "The bit field width must be a multiple of 8."
		//const ultralong<900> n10{ "1234567890987654321" };

		// "Incorrect format for writing a negative number."
		//const ultralong<w> n11{ "12-3456789Q987654321" };

		// "A character cannot be converted to a digit."
		//const ultralong<w> n12{ "123456789Q987654321" };

		//"The type does not have the ability to accommodate the number."
		//const ultralong<16> n13{ std::vector<unsigned>{1,2,3,4,5,6,7,8,9,0,9,8,7,6,5,4,3,2,1} };
		//const ultralong<160> n10{ "1234567890987654321" };

		//"The calculation result is too large for the target type."
		//const ultralong<16> a{ {9,9} };
		//ultralong<16> b{ "1" };
		//b += a;
	}
	catch (const std::exception& err)
	{
		std::cout << err.what();
	}

	
	return 0;
}