// homework_04_02_01.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "point.h"

int main()
{
	constexpr unsigned max_num{ 5 };
	for (unsigned i{ 0 }; i < max_num; i++) {
		const point my_point(static_cast<double>(i), 2.0 * i);
		print_point(my_point);
	}
	return 0;
}