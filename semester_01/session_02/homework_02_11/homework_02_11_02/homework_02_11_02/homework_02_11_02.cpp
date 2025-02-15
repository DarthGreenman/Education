// homework_02_11_02.cpp 

#include <iostream>
#include <string>

#include "..\..\lib\Addressing_someone\Addressing_someone\Greeter.h"
#include "..\..\lib\Addressing_someone\Addressing_someone\Leaver.h"
#include "..\..\lib\localisation.h"

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