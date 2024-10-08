// homework_02_09_02.cpp

/*
 * Необходимо переопределить операторы для класса Fraction из предыдущего задания:
 * сложение, вычитание, умножение, деление, унарный минус,
 * инкремент постфиксный и префиксный, декремент постфиксный и префиксный
 */

#include <iostream>
#include <memory>
#include <vector>

#include "Fraction.h"
#include "localisation.h"
#include "Input.h"

int main()
{
    const bag::Console_localisation locale{
        bag::Console_localisation::Code_pages::rus
    };
    using std::cout;
    using std::cin;

    const int amount_of_numbers{ 2 };
    std::vector<bag::Fraction> f(amount_of_numbers);

    for (size_t i{}; i < amount_of_numbers; ++i) {
        int num{}, den{};
        for(; num <= 0;) {
            num = bag::get_input_value<int>("Введите числитель дроби " +
                std::to_string(i + 1) + ": ");
        }

        for (; den <= 0;) {
            den = bag::get_input_value<int>("Введите знаменатель дроби " +
                std::to_string(i + 1) + ": ");
        }

        try {
            f[i] = *std::make_unique<bag::Fraction>(num, den).get();
        }
        catch (const std::exception& e) {
            std::cerr << e.what();
            continue;
        }
    }

    cout << f[0] << " + " << f[1] << " = " << f[0] + f[1] << '\n';
    cout << f[0] << " - " << f[1] << " = " << f[0] - f[1] << '\n';
    cout << f[0] << " * " << f[1] << " = " << f[0] * f[1] << '\n';
    cout << f[0] << " / " << f[1] << " = " << f[0] / f[1] << '\n';
    cout << "++" << f[0] << " * " << f[1] << " = " << ++f[0] * f[1] << '\n';
    cout << "Значение дроби 1 = " << f[0] << '\n';
    cout << f[0] << "--" << " * " << f[1] << " = " << f[0]-- * f[1] << '\n';
    cout << "Значение дроби 1 = " << f[0] << '\n';

    return 0;
}