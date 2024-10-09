// homework_01_03_01.cpp
// ЗАДАНИЕ 1
// Напишите программу, которая выводит таблицу истинности для
// логических операторов ||, &&, с применением логических выражений
//

#include <iostream>
#include <locale>

int main()
{
	setlocale(LC_ALL, "Russian");

	using namespace std;

	cout << std::boolalpha;

	const bool t{ true },
		f{ false };

	cout << "Оператор: ||\n\n";
	cout << "A\t" << "B\t" << "A || B\n";
	
	cout << t << '\t' << t << '\t' << (t || t) << '\n';
	cout << f << '\t' << t << '\t' << (f || t) << '\n';
	cout << t << '\t' << f << '\t' << (t || f) << '\n';
	cout << f << '\t' << f << '\t' << (f || f) << '\n';

	cout << "\n\nОператор: &&\n\n";
	
	cout << "A\t" << "B\t" << "A && B\n";
	cout << t << '\t' << t << '\t' << (t && t) << '\n';
	cout << f << '\t' << t << '\t' << (f && t) << '\n';
	cout << t << '\t' << f << '\t' << (t && f) << '\n';
	cout << f << '\t' << f << '\t' << (f && f) << '\n';
}