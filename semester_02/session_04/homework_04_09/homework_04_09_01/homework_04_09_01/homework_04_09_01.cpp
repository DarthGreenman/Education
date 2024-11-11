// homework_04_09_01.cpp : Этот файл содержит функцию "main".
// Вам нужно реализовать шаблонную функцию, которая перемещает содержимое одного std::vector в другой.
// std::vector <std::string> one = { "test_string1", "test_string2" };
// std::vector <std::string> two;
// move_vectors(...)

#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include <algorithm>

template<typename T>
auto move_vector(std::vector<T>& vector) 
{
	return std::vector<T>{std::move(vector)};
}

template<typename T>
auto print(const std::vector<T>& one, const std::vector<T>& two)
{
	using namespace std;
	auto pr = [](const std::string& elem) { cout << "value = " << elem << " address = " << &elem << '\n';	};

	cout << "\nElements of vector 1:\n";
	for_each(std::cbegin(one), std::cend(one), pr);
	cout << "\nElements of vector 2:\n";
	for_each(std::cbegin(two), std::cend(two), pr);
}

int main()
{

	std::vector<std::string> one{ "test_string1", "test_string2" };
	std::vector<std::string> two{};

	std::cout << "Before moving.";
	print(one, two);
	
	two = move_vector(one);
	std::cout << "\n\nAfter moving.";
	print(one, two);

	return 0;
}
