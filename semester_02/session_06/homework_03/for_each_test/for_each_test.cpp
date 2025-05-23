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
		vector<
			tuple<
			size_t, int, int, int, 
			chrono::duration<double, milli>, 
			chrono::duration<double, milli>,
			chrono::duration<double, milli>
			>
		> data{};

		for (const auto& count : initializer_list<size_t>
			{ 1'000,10'000,100'000,1'000'000, 10'000'000, 100'000'000, 1'000'000'000 })
		{
			const auto vec = utl::make_vector_sequential_values(count, 0);
			using value_type = typename decay_t<decltype(vec)>::value_type;

			std::tuple<value_type, value_type, value_type> sum{};
			auto add = [&sum](value_type value) { std::get<0>(sum) += value; };

			auto ts = utl::get<decltype(add)>(std::for_each,
				cbegin(vec), cend(vec), add
			);
			auto tm = utl::get<void>(my::for_each_m,
				cbegin(vec), cend(vec), [&sum](value_type value) { std::get<1>(sum) += value; }
			);
			auto ta = utl::get<void>(my::for_each,
				cbegin(vec), cend(vec), [&sum](value_type value) { std::get<2>(sum) += value; }
			);
											
			data.emplace_back(count, 
				std::get<0>(sum), std::get<1>(sum), std::get<2>(sum), 
				std::move(ts), std::move(tm), std::move(ta));
		}

		constexpr auto width = 15u;
		cout << setiosflags(ios::left);
		cout << "\n\n\nDemonstration of the speed of execution of the operation of summing elements of a sequence:\n\n";
		cout << setw(width) << "Number" 
			<< setw(width) << "Sum std" << setw(width) << "Sum m" << setw(width) << "Suma" 
			<< setw(width) << "Time std" << setw(width) << "Time m" << "Time a\n";
				
		std::for_each(std::cbegin(data), std::cend(data), [](typename std::decay_t<decltype(data)>::value_type value)
			{
				auto&& [count, sum1, sum2, sum3, ts, tm, ta] = value;
				cout << setw(width) << count
					<< setw(width) << sum1 << setw(width) << sum2 << setw(width) << sum3
					<< setw(width) << ts << setw(width) << tm << ta << '\n';
			}
		);
	}

	std::cout << '\n';
	std::system("pause");
	return 0;
}

