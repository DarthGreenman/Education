// selection_sort_test.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "include\selection_sort.h"

#include <cstdlib>
#include <iostream>
#include <type_traits>
#include <vector>

namespace utl
{
	template<typename T>
	auto print(const std::vector<T>& vec, const char* heading)
	{
		std::cout << heading;
		for (const auto& elem : vec)
			std::cout << elem << ' ';
	}

	template<typename T, std::size_t N>
	auto print(T(&array)[N], const char* heading)
	{
		std::cout << heading;
		for (decltype(N) i{}; i < N; ++i)
			std::cout << array[i] << ' ';
	}
} // namespace utl

int main()
{
	using namespace utl;
	///////////////////////////////////////////////////////////////////////////////////////////////
	std::vector<int> vec{ 2,88,91,87,3,59,0,53,49,-29,16,4,27,28,0,89,2,25,74,-123,-456 };
	print(vec, "Unordered vector: ");

	my::sort(std::begin(vec), std::end(vec));
	print(vec, "\nSorted vector in ascending order: ");

	using Type_elem_vec = typename decltype(vec)::value_type;
	my::sort(std::begin(vec), std::end(vec), std::less<Type_elem_vec>{});
	print(vec, "\nSorted vector in descending order: ");

	///////////////////////////////////////////////////////////////////////////////////////////////
	double arr[]{ 24.6,66.9,20.0,79,30.1,16,-19,-19,62,94,59,0,7,59,90,84,60,95,-62 };
	print(arr, "\n\nUnordered array: ");

	my::sort(arr);
	print(arr, "\nSorted array in ascending order: ");

	using Type_elem_array = typename std::remove_all_extents<decltype(arr)>::type;
	my::sort(arr, std::less<Type_elem_array>{});
	print(arr, "\nSorted array in descending order: ");

	std::system("pause");
	return 0;
}