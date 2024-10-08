// Real hash function.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "my_hash.h"
#include "my_input.h"
#include "my_localisation.h"

#include <__msvc_string_view.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>

auto get_input(std::string_view invitation)
{
	using namespace std;
	cout << invitation;
	string ret{}; cin >> ret;
	return ret;
}

int main()
{
    const my::Console_localisation<my::Code_pages::rus> local{};
	using size_type = std::size_t;
	using std::cout;
	
	size_type p{};
	for (; p = my::get_input_value<size_type>("Введите простое число p: "), !my::is_prime(p);)
		cout << "Повторите ввод, число должно быть простое.\n";

	const auto n = my::get_input_value<size_type>("Введите целое число n: ");
	
	constexpr std::string_view mes{ "exit" };
	const auto quit = 
		my::hash_function(std::cbegin(mes), std::cend(mes), std::make_pair(p, n));
	
	for (;;)
	{
		const auto mes = get_input("\nВведите строку: ");
		const auto hash = 
			my::hash_function(std::cbegin(mes), std::cend(mes), std::make_pair(p, n));
		cout << "Хэш строки " << mes << " = " << hash << '\n';
		if (hash == quit)
			break;
	}

	std::system("pause");
    return 0;
}


