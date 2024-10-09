// homework_01_04_03.cpp

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

int get_input_value(const std::string&);

int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	std::cout << "Введите число в диапазоне от " <<
		std::numeric_limits<int>::min() << " до " <<
		std::numeric_limits<int>::max() << ": ";

	const std::string message{ "\nВы ввели некорректное значение для типа 'int'."
		"\nПовторите ввод, и будьте внимательными: " };
	
	int number{ 0 };
	number = get_input_value(message);
	std::cout << "\nВы ввели число: " << number << '\n';

	const unsigned short MULTIPLIER_MAX{ 10 };
	for (unsigned short multiplier = 1;
		multiplier <= MULTIPLIER_MAX; ++multiplier) {
		std::cout << number << " x "
			<< multiplier << " = " 
			<< number * multiplier 
			<< '\n';
	}
	return 0;
}

int get_input_value(
	const std::string& message
) {
	while (true) {
		int temp{ 0 };
		std::cin >> temp;
		if (std::cin.good()) {
			return temp;
		}
		else if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
			std::cout << message;
		}
		else /* if (std::cin.bad()) */ {
			throw std::exception{"\nПроизошла фатальная ошибка!\n"};
		}
	}
}