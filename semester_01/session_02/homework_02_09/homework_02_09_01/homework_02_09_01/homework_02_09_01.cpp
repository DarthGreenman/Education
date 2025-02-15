// homework_02_09_01.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <exception>

#include "Fraction.h"
#include "localisation.h"

int main()
{
    bag::Console_localisation locale(bag::Console_localisation::Code_pages::rus);
    using std::cout;

    bag::Fraction f1;
    bag::Fraction f2;

    try {
        f1 = bag::make_fraction(2, 4);
        f2 = bag::make_fraction(1, 2);
    }
    catch (const std::exception& e) {
        std::cerr << e.what();
        return 1;
    }
   
    cout << f1 << ((f1 == f2) ? " == " : " not == ") << f2 << '\n';
    cout << f1 << ((f1 != f2) ? " != " : " not != ") << f2 << '\n';
    cout << f1 << ((f1 < f2) ? " < " : " not < ") << f2 << '\n';
    cout << f1 << ((f1 > f2) ? " > " : " not > ") << f2 << '\n';
    cout << f1 << ((f1 <= f2) ? " <= " : " not <= ") << f2 << '\n';
    cout << f1 << ((f1 >= f2) ? " >= " : " not >= ") << f2 << '\n';
    
    return 0;
}