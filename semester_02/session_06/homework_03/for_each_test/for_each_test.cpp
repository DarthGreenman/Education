// for_each_test.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "include/for_each.h"

#include <algorithm>
#include <chrono>
#include <concepts>
#include <cstdlib>
#include <initializer_list>
#include <iomanip>
#include <iosfwd>
#include <iostream>
#include <ratio>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

//#define DEBUG

namespace utl
{
	template<std::integral Type>
	auto make_vector_sequential_values(std::size_t size, Type init_value = Type{})
	{
		std::vector<Type> vec{};
		vec.reserve(size);
		for (decltype(size) i{}; i < size; ++i)
			vec.push_back(init_value++);
		return vec;
	}

	template<typename ResultType, std::forward_iterator ForwardIter, typename UnaryFunc>
	auto get(ResultType(*func)(ForwardIter, ForwardIter, UnaryFunc), ForwardIter first, ForwardIter last,
		UnaryFunc unary_func) -> std::chrono::duration<double, std::milli>
	{
		const auto s = std::chrono::high_resolution_clock::now();
		func(first, last, unary_func);
		const auto f = std::chrono::high_resolution_clock::now();
		return f - s;
	}
} // namespace utl

int main()
{
	using namespace std;
	{
		const auto vec = utl::make_vector_sequential_values(50, 0);
		cout << "Demonstration of the sequence of operations:\n";
		my::for_each(begin(vec), end(vec), [](typename decltype(vec)::value_type value)
			{
				cout << value << ' ';
			}
		);
	}

	{
		vector<tuple<size_t, int, chrono::duration<double, milli>, chrono::duration<double, milli>>> data{};

		for (const auto& count : initializer_list<size_t>
			{ 1'000,10'000,100'000,1'000'000,10'000'000,100'000'000,1'000'000'000 })
		{
			const auto vec = utl::make_vector_sequential_values(count, 0);
			using value_type = typename decay_t<decltype(vec)>::value_type;

			std::pair<value_type, value_type> sum{};
			auto add = [&sum](value_type value) { sum.first += value; };

			auto ts = utl::get<decltype(add)>(std::for_each,
				cbegin(vec), cend(vec), add
			);
			auto tm = utl::get<void>(my::for_each,
				cbegin(vec), cend(vec), [&sum](value_type value) { sum.second += value; }
			);
											
			data.emplace_back(count, sum.second - sum.first, std::move(ts), std::move(tm));
		}

		cout << setiosflags(ios::left);
		cout << "\n\n\nDemonstration of the speed of execution of the operation of summing elements of a sequence:\n\n";
		cout << setw(20) << "Number of elem." << setw(20) << "Checksum" << setw(20) << "Time std" << setw(20) << "Time my" << "Time my / Time std\n";	
				
		std::for_each(std::cbegin(data), std::cend(data), [](typename std::decay_t<decltype(data)>::value_type value)
			{
				auto&& [count, check, ts, tm] = value;
				cout << setw(20) << count << setw(20) << check << setw(20) << ts << setw(20) << tm << tm / ts << '\n';
			}
		);
	}

	std::cout << '\n';
	std::system("pause");
	return 0;
}

