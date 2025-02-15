// homework_01_04_01.cpp

#include <iostream>
#include <limits>

#define NOMINMAX
#include <Windows.h>

/*
 int max{ std::numeric_limits<int>::max() };    не работатет
 ******************************************************************************
 #define NOMINMAX
 #include <Windows.h>
 int max{ std::numeric_limits<int>::max() };    работате
 ******************************************************************************
 #under max
 int max{ std::numeric_limits<int>::max() };    работает
 ******************************************************************************
 int max{ (std::numeric_limits<int>::max)() };  работает
 */

int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int sum{ 0 };

	std::cout << "Введите число в диапазоне от " <<
		std::numeric_limits<int>::min() << " до " <<
		std::numeric_limits<int>::max() << " или число '0', чтобы закончить:\n";
	
	for (int num{ 0 }; std::cin >> num && num != 0;) {
		sum += num;
	}
	if (std::cin.fail()) {
		std::cout << "\nВы ввели некорректное значение для типа 'int'."
			"\nПерезапустите программу, и будьте внимательными.";
	} 
	else if (sum != 0){
		std::cout << "\nСумма цифр: " << sum;
	}
	else {
		std::cout << "\nВы не ввели ни одного числа отличного от нуля."
			"\nПерезапустите программу, и введите хотя бы два числа отличных от нуля.";
	}
	return 0;
}