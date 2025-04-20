// for_each_test.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "include/for_each.h"

#include <algorithm>
#include <chrono>
#include <concepts>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <ratio>
#include <vector>

//#define DEBUG

namespace utl
{
	template<std::integral T>
	auto make_vector_sequential_values(std::size_t size, T init_value = T{})
	{
		std::vector<T> result{};
		result.reserve(size);
		for (decltype(size) i{}; i < size; ++i)
			result.push_back(init_value++);
		return result;
	}
} // namespace utl

int main()
{
#ifdef DEBUG
	const auto vec1 = utl::make_vector_sequential_values<std::size_t, 1'000>();
	my::for_each(std::begin(vec1), std::end(vec1), [](typename decltype(vec1)::value_type value)
		{
			std::cout << value << ' ';
		}
	);
#endif // DEBUG

#ifndef DEBUG
	const std::initializer_list<std::size_t> count_list{ 
		1'000,10'000,100'000,1'000'000,10'000'000,100'000'000 };

	for (const auto& count : count_list)
	{
		const auto vec = utl::make_vector_sequential_values<std::size_t>(count);
		typename decltype(vec)::value_type sum{};
		auto print = [&sum](const std::chrono::time_point<std::chrono::high_resolution_clock>& start,
			const std::chrono::time_point<std::chrono::high_resolution_clock>& finish,
			const char* title)
			{
				std::cout << title << sum << ",\tcalculation time = "
					<< std::chrono::duration<double, std::milli>{ finish - start };
			};

		std::cout << "\n\nSum of array " << count <<" elements calculated by algorithm : ";

		auto start = std::chrono::high_resolution_clock::now();
		std::for_each(std::begin(vec), std::end(vec), [&sum](typename decltype(vec)::value_type value)
			{
				sum += value;
			}
		);
		print(start, std::chrono::high_resolution_clock::now(), "\n - std::for_each = ");

		sum = 0;
		start = std::chrono::high_resolution_clock::now();
		my::for_each(std::begin(vec), std::end(vec), [&sum](typename decltype(vec)::value_type value)
			{
				sum += value;
			}
		);
		print(start, std::chrono::high_resolution_clock::now(), "\n - my::for_each  = ");
	}
#endif // !DEBUG

	std::cout << '\n';
	std::system("pause");
	return 0;
}
