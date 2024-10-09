// homework_02_10_01.cpp: определяет точку входа для приложения.
//

#include "Localisation.h"
#include <iostream>
#include <string>

int main()
{
	bag::Console_localisation locale{
		bag::Console_localisation::Code_pages::rus
	};

	std::cout << "Введите имя: ";
	std::string name{};
	std::cin >> name;

	std::cout << "Здравствуйте, " << name << '!';

return 0;
}
