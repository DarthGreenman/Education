// ultralong.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "includes/my_test.h"
#include "includes/my_ultralong.h"
#include "includes/my_ultralong_ext.h"

#include <exception>
#include <iostream>
#include <random>



int main()
{
	try
	{		
		std::mt19937 engine;
		using namespace my;
		test(engine, 10, sum<std::size_t>{}, sum<my::ultralong<1024>>{}, '+');
		test(engine, 15, mul<std::size_t>{}, mul<my::ultralong<1024>>{}, '*');		
	}
	catch (const std::exception& err)
	{
		std::cout << err.what();
	}

	return 0;
}