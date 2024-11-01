// homework_04_09_01.cpp : Этот файл содержит функцию "main".
// Вам нужно реализовать шаблонную функцию, которая перемещает содержимое одного std::vector в другой.
// std::vector <std::string> one = { "test_string1", "test_string2" };
// std::vector <std::string> two;
// move_vectors(...)

#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

template<typename T>
std::vector<T> move_vector(std::vector<T>& vector) 
{
	return static_cast<std::vector<T>&&>(vector);
}

int main()
{
	std::vector<std::string> one_s{ "test_string1", "test_string2" };
	auto two_s{ move_vector(one_s) };

	std::vector<int> one_i{ -234,0,1,-34,6789 };
	auto two_i{ move_vector(one_i) };
	
	return 0;
}
