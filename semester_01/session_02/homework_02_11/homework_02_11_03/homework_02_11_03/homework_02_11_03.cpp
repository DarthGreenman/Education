// homework_02_11_03.cpp: определяет точку входа для приложения.
//

#include <iostream>
#include <string>

#include "Localisation.h"
#include "Greeter.h"
#include "Leaver.h"

int main()
{
    using std::cout;
    using std::cin;
    using std::cerr;

    const bag::Console_localisation locale{
        bag::Console_localisation::Code_pages::rus
    };

    cout << "Введите имя: ";
    std::string name{};
    cin >> name;

    bag::Greeter hello;
    cout << hello.greet(name);

    bag::Leaver goodbye;
    cout << '\n' << goodbye.leave(name) << '\n';

    std::system("pause");
    return 0;
}