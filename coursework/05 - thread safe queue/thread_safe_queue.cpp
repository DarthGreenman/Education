// thread_safe_queue.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "includes/thread_pool.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <functional>
#include <future>
#include <iostream>
#include <thread>
#include <type_traits>
#include <vector>

namespace test
{
	template<typename T,
		typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	decltype(auto) add(T a, T b)
	{
		std::cout << __FUNCTION__ << '\t' << std::this_thread::get_id() << '\n';
		return a + b;
	};

	template<typename T,
		typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	decltype(auto) sub(T a, T b)
	{
		std::cout << __FUNCTION__ << '\t' << std::this_thread::get_id() << '\n';
		return a - b;
	};

	template<typename T,
		typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	decltype(auto) mul(T a, T b)
	{
		std::cout << __FUNCTION__ << '\t' << std::this_thread::get_id() << '\n';
		return a * b;
	};

	template<typename Container>
	auto print(const Container& vec)
	{
		using value_type = typename Container::value_type;
		std::for_each(std::cbegin(vec), std::cend(vec), [](const value_type& value)
			{
				std::cout << value << ", ";
			}
		);
	}

	template<typename T>
	struct acc
	{
		void operator()(T value) { sum += value; }
		T sum{};
	}; // struct acc

} // namespace test

int main()
{
	using namespace std::chrono_literals;;
	using func_prepare = std::function<int(int, int)>;
	using value_type = typename func_prepare::result_type;

	multitask::thread_pool<value_type> prepare{};
	std::vector<value_type> data_one{};
	std::vector<value_type> data_two{};

	constexpr typename std::vector<value_type>::size_type num_iter{ 10 };
	constexpr typename std::vector<value_type>::size_type num_func{ 3 };

	data_one.reserve(num_iter * num_func);
	data_two.reserve(num_iter * num_func);
	for (typename std::vector<value_type>::size_type count{}; count < num_iter;
		++count, std::this_thread::sleep_for(1s))
	{
		auto f1 = prepare.submit(func_prepare{ test::add<value_type> }, 0, count);
		data_one.push_back(f1.get());
		auto f2 = prepare.submit(func_prepare{ test::sub<value_type> }, num_iter - 1, count);
		data_two.push_back(f2.get());
	}

	std::cout << "\nVector one: "; test::print(data_one);
	std::cout << "\nVector two: "; test::print(data_two);


	// ДЕМОНСТРАЦИЯ РАБОТЫ ПУЛА С СТАНДАРТНЫМИ АЛГОРИТМАМИ

	// template<class BidirIt> void reverse(BidirIt first, BidirIt last);
	using iter = typename std::vector<value_type>::iterator;
	using func_reverse = std::function<void(iter, iter)>;

	multitask::thread_pool<typename func_reverse::result_type> reverse{};
	auto r1 = reverse.submit(func_reverse{ std::reverse<iter> }, std::begin(data_one), std::end(data_one));
	auto r2 = reverse.submit(func_reverse{ std::reverse<iter> }, std::begin(data_two), std::end(data_two));
	r1.get(); r2.get();

	std::cout << "\n\nData reverse";
	std::cout << "\nVector one: "; test::print(data_one);
	std::cout << "\nVector two: "; test::print(data_two);

	// template<class InputIt, class UnaryFunc> UnaryFunc for_each(InputIt first, InputIt last, UnaryFunc f);
	using const_iter = typename std::vector<value_type>::const_iterator;
	using func_accumulate = std::function<test::acc<value_type>(const_iter, const_iter, test::acc<value_type>)>;

	multitask::thread_pool<typename func_accumulate::result_type> accumulate{};
	
	auto s1 = accumulate.submit(func_accumulate{ std::for_each<const_iter, test::acc<value_type>> },
		std::cbegin(data_one), std::cend(data_one), test::acc<value_type>{});
	auto s2 = accumulate.submit(func_accumulate{ std::for_each<const_iter, test::acc<value_type>> },
		std::cbegin(data_two), std::cend(data_two), test::acc<value_type>{});

	std::cout << "\n\nSum of vector elements";
	std::cout << "\nVector one: "; std::cout << s1.get().sum;
	std::cout << "\nVector two: "; std::cout << s2.get().sum;

	std::cout << '\n';
	std::system("pause");
	return 0;
}