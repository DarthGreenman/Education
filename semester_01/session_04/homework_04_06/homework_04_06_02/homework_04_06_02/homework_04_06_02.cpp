// homework_04_06_02.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "lib/localisation.h"
#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
	const bag::Console_localisation locale{
		bag::Console_localisation::Code_pages::rus
	};

	std::size_t count{};
	std::cout << "Введите количество цифр: "; std::cin >> count;

	std::vector<int> numbers;
	numbers.reserve(count);
	for (std::size_t i{}; i < count; ++i) {
		int number{};
		std::cout << "Введите целочисленное значение элемента [" << i + 1 << "]: "; 
		std::cin >> number;
		numbers.push_back(number);
	}

	std::cout << "\nНеотсортированный массив с неуникальными элементами: ";
	std::for_each(std::cbegin(numbers), std::cend(numbers),
		[](int Elem) -> void { std::cout << Elem << " "; }	);

	std::cout << "\n\nОтсортированный массив с уникальными элементами: ";
	std::sort(std::begin(numbers), std::end(numbers),
		[](int Lhs, int Rhs) -> bool { return Lhs > Rhs; });
	numbers.erase(
		std::unique(std::begin(numbers), std::end(numbers)),
		std::end(numbers));
	
	std::for_each(std::cbegin(numbers), std::cend(numbers),
		[](int Elem) -> void { std::cout << Elem << " "; });
	
	std::cout << '\n';
	std::system("pause");
	return 0;
}