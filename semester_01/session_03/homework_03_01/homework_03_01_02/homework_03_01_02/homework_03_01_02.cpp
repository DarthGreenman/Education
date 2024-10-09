// homework_03_01_02.cpp : Этот файл содержит функцию "main".
/*
* Нужно написать функцию, которая принимает на вход отсортированный массив целых чисел и целое число.
* Целое число назовём точкой отсчёта. Функция должна посчитать количество элементов в массиве,
* которые больше, чем переданное в функцию целое число — то есть больше, чем точка отсчёта.
* Требуемая алгоритмическая сложность: время O(log n), дополнительная память O(1).
* [14, 16, 19, 32, 32, 32, 56, 69, 72]
* [14, 32, 32, 32, 32, 32, 32, 32, 72]
* [72, 69, 56, 32, 32, 32, 19, 16, 14]
* [72, 32, 32, 32, 32, 32, 32, 32, 14]
*/

#include <iostream>
#include <algorithm>

#include "localisation.h"
#include "utilities.h"

void view_sequence(const int* begin, const int* end, int point, size_t num);
void view_sequence(const int* begin, const int* end, const std::string& message);

int main()
{
	const seq::Console_localisation locale{ 
		seq::Console_localisation::Code_pages::rus
	};
	using std::cout;
	constexpr int arr[]{ 14, 16, 19, 32, 32, 32, 56, 69, 72 };
	

	cout << "Oтсортированный массив целых чисел: ";
	for (auto x = std::begin(arr); x != std::end(arr); ++x) {
		std::cout << *x << ' ';
	}
	cout << "\nВведите точку отсчёта: ";
	int point{}; std::cin >> point;
	if (auto&& [first, last] = seq::search::binary(arr, point);
		first != last)
	{
		if (const bool is_ascending{ *std::begin(arr) < *(std::end(arr) - 1) };
			is_ascending)
		{
			view_sequence(last, std::end(arr), 
				point, std::end(arr) - last);
		}
		else {
			view_sequence(std::begin(arr), first, 
				point, first - std::begin(arr));
		}	
	}
	else {
		cout << "Данная точка отсчета отсутствует в списке.\n";
	}

	cout << "\nПроанализировать все элементы массива на меньшие и большие значения?\n"
		<< "Для продолжения нажмите Y, для выхода - любую клавишу: ";
	char answer{};
	if (std::cin >> answer; std::toupper(answer) == 'Y')
	{
		cout << "\nАнализ работы алгоритма - для всех значений массива находим\n"
			"меньшие и большие элементы.\n";
		int tmp{ (std::numeric_limits<int>::max)() };
		for (auto ptr = std::begin(arr); ptr != std::end(arr); ++ptr)
		{
			if (tmp == *ptr) {
				continue;
			}

			cout << "\nДля элемента " << *ptr << '\n';
			if (auto&& [first, last] = seq::search::binary(arr, *ptr);
				first != last)
			{
				const bool is_ascending{ *std::begin(arr) < *(std::end(arr) - 1) };
				/* Значения расположенные слева от текущего */
				if (const auto num = first - std::begin(arr); num != 0)
				{
					view_sequence(std::begin(arr), first,
						is_ascending ? "значения, которые меньше текущего: " :
						"значения, которые больше текущего: ");
				}

				/* Значения расположенные справа от текущего */
				if (const auto num = std::end(arr) - last; num != 0)
				{
					view_sequence(last, std::end(arr),
						is_ascending ? "значения, которые больше текущего: " :
						"значения, которые меньше текущего: ");
				}
			}
			tmp = *ptr;
		}
		std::system("pause");
	}
	
	return 0;
}

void view_sequence(const int* begin, const int* end, int point, size_t num)
{
	std::cout << "Количество элементов в массиве больших, чем " << point << ": " << num;
	if (num != 0) {
		std::cout << "\nзначения элементов: ";
		for (auto x = begin; x != end; ++x) {
			std::cout << *x << ' ';
		}
	}
	std::cout << '\n';
}

void view_sequence(const int* begin, const int* end, const std::string& message)
{
	std::cout << message;
	int tmp{ (std::numeric_limits<int>::max)() };
	for (auto x = begin; x != end; ++x) {
		if (tmp != *x) {
			std::cout << *x << ' ';
		}
		tmp = *x;
	}
	std::cout << '\n';
}

