// homework_01_03_01.cpp
// ЗАДАНИЕ 1
// Напишите программу, которая выводит таблицу истинности для
// логических операторов ||, &&, с применением логических выражений
//

#include <iostream>
#include <locale>

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	
	std::cout << std::boolalpha;

	const size_t size{ 4 };
	const bool left_val[size]{
		true,
		false,
		true,
		false
	};
	const bool right_val[size]{
		true,
		true,
		false,
		false
	};

	cout << "Оператор: ||\n\n";
	cout << "A\t" << "B\t" << "A || B\n";
	for (size_t i = 0; i < size; ++i)
	{
		cout << left_val[i] << '\t'
			<< right_val[i] << '\t' 
			<< (left_val[i] || right_val[i]) << '\n';
	}

	cout << "\n\nОператор: &&\n\n";
	cout << "A\t" << "B\t" << "A && B\n";
	for (size_t i = 0; i < size; ++i)
	{
		cout << left_val[i] << '\t'
			<< right_val[i] << '\t'
			<< (left_val[i] && right_val[i]) << '\n';
	}
}