// homework_02_03_01.cpp

#include <iostream>
#include <utility>
#include <limits>
#include <exception>

#include "calculator.h"
#include "localisation.h"

template <typename T>
T get_input_value(const std::string & =
    "Введите правильное цифровое значение: "
);

void view(const std::pair<double, bool>&);

int main()
{
    bag::console_localisation localisation(1251);
    using std::cout;
    using std::cin;
    using calculator::operand_order::left_right;
    using calculator::operand_order::right_left;
    using calculator::operand_position::left;
    using calculator::operand_position::right;

    for (char answer{ 'y' }; answer == 'y';) {
        cout << "Введите число \nстоящее слева от знака операции: ";
        const double number_left{ get_input_value<double>() };
        cout << "Введите число \nстоящее справа от знака операции: ";
        const double number_right{ get_input_value<double>() };

        calculator numbers{ number_left, number_right };

        cout << '\n'
            << numbers.get_value(left) << " + "
            << numbers.get_value(right) << " = "
            << numbers.add();
        /*************************************************************************/
        cout << '\n'
            << numbers.get_value(left) << " - "
            << numbers.get_value(right) << " = "
            << numbers.subtract(left_right);
        cout << '\n'
            << numbers.get_value(right) << " - "
            << numbers.get_value(left) << " = "
            << numbers.subtract(right_left);
        /*************************************************************************/
        cout << '\n'
            << numbers.get_value(left) << " * "
            << numbers.get_value(right) << " = "
            << numbers.multiply();
        /*************************************************************************/
        cout << '\n'
            << numbers.get_value(left) << " / "
            << numbers.get_value(right);
        view(numbers.divide(left_right));
        cout << '\n'
            << numbers.get_value(right) << " / "
            << numbers.get_value(left);
        view(numbers.divide(right_left));
        
        cout << "\n\nДля продолжения нажмите 'y', \nдля выхода любую другую клавишу: ";
        cin >> answer;
        cout << "\n\n";
    }

    return 0;;
}

void view(const std::pair<double, bool>& res) {
    if (!res.second) {
        std::cout << " = " 
            << res.first;
    }
    else {
        std::cout << " = деление на ноль недопустимо!";
    };
}

template <typename T>
T get_input_value(const std::string& message)
{
    while (true) {
        T result{};
        std::cin >> result;
        if (std::cin.good()) {
            return result;
        }
        else if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(),
                '\n');
            std::cout << message;
        }
        else /* if (std::cin.bad()) */ {
            throw std::exception{ "\nПроизошла фатальная ошибка!\n" };
        }
    }
}