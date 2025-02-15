// Naive hash function.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
// Наивная хэш-функция для строки

#include "my_hash.h"
#include "my_localisation.h"

#include <__msvc_string_view.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

int main()
{
	const my::Console_localisation<my::Code_pages::rus> local{};

	auto get_input = [](std::string_view invitation) {
		std::cout << invitation;
		std::string ret{}; std::cin >> ret;
		return ret;
	};

	for (constexpr auto quit = my::hash_function("exit");;)
	{
		const auto mes = get_input("\nВведите строку: ");
		const auto hash = my::hash_function(std::cbegin(mes), std::cend(mes));
		std::cout << "Наивный хэш строки " << mes << " = " << hash << '\n';
		if (hash == quit)
			break;
	}

	std::system("pause");
    return 0;
}
