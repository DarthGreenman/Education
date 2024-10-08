// Remove element mutable dynamic array.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
// Удаление первого элемента из изменяемого динамического массива

#include "my_dynamic_array.h"
#include "my_input.h"
#include "my_localisation.h"

#include <__msvc_string_view.hpp>
#include <algorithm>
#include <cstdlib>
#include <iostream>

namespace my
{
	using const_iterator = typename my::My_dynamic_array<int>::const_iterator;
	void print_dynamic_array(const_iterator first, const_iterator end, const_iterator last)
	{
		for (; first != last; ++first)
			first < end ? std::cout << *first << ' ' : std::cout << "_ ";
	}

	enum class answer : char { no = 'N', yes = 'Y' };

	auto get_correct_answer(std::string_view message)
	{
		char ret_val{};
		std::cout << message;
		for (; std::cin >> ret_val, toupper(ret_val) != static_cast<char>(answer::yes) &&
			std::toupper(ret_val) != static_cast<char>(answer::no);)
		{
			std::cout << "Повторите ввод, нажмите Y или N:";
		}
		return ret_val;
	}
}

int main()
{
	const my::Console_localisation local{
		my::Console_localisation::Code_pages::rus
	};
	using std::cout;
	if (const auto actual_size = my::get_input_value<std::size_t>("Введите фактичеcкий размер массива: "),
		logical_size = my::get_input_value<std::size_t>("Введите логический размер массива: ");
		logical_size <= actual_size)
	{
		my::My_dynamic_array<int> array(logical_size, actual_size);
		std::for_each(std::begin(array), std::end(array),
			[](my::My_dynamic_array<int>::value_type& value)
			{
				static std::size_t i{};
				cout << "Введите [" << i++ << "]: ";
				value = my::get_input_value<my::My_dynamic_array<int>::value_type>();
			}
		);

		constexpr std::string_view message{ "Динамический массив: " };
		cout << message;
		my::print_dynamic_array(std::cbegin(array), std::cend(array),
			std::cbegin(array) + array.capacity());

		using std::toupper;
		using answer = typename my::answer;
		for (char go_on{ static_cast<char>(answer::yes) };
			toupper(go_on) == static_cast<char>(answer::yes); )
		{
			go_on = my::get_correct_answer("\nДля удаления первого элемента нажмите Y,\nдля выхода - N: ");
			if (toupper(go_on) == static_cast<char>(answer::yes)) {
				if (const auto elem = array.erase();
					elem == std::cend(array))
				{
					cout << "Вы удалили последний элемент массива.\n";
					go_on = static_cast<char>(answer::no);
					break;
				}
				cout << message;
				my::print_dynamic_array(std::cbegin(array), std::cend(array),
					std::cbegin(array) + array.capacity());
			}
		}
	}
	else {
		cout << "Ошибка! Логический размер массива не может превышать фактический!\n";
	}

	std::system("pause");
	return 0;
}