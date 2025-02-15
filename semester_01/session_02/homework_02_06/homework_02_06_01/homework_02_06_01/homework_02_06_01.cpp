// homework_02_06_01.cpp

#include <iostream>
#include <cmath>

#include "localisation.h"
#include "math_functions.h"

void view(double, double, double, math::Operation);

template <typename T>
T get_input_value(std::istream&,
    const std::string& = "Введите правильное цифровое значение: ");

int main()
{
	bag::Console_localisation locale(1251);
	using std::cout;
	using std::cin;

	cout << "Введите первое число: ";
    double lhs{ get_input_value<double>(cin)};
	
    cout << "Введите второе число: ";
    double rhs{ get_input_value<double>(cin) };

	cout << "Введите операцию\n"
		"+ - сложение\n" 
		"- - вычитание\n"
		"* - умножение\n"
		"/ - деление\n"
		"^ - возведение в степень\n"
		": ";
       
    for (char op{}; cin >> op; ) {
        switch (op)
        {
            using math::Operation;
            case static_cast<char>(Operation::add):
                view(lhs, rhs, math::add(lhs, rhs), Operation::add);
                break;
            case static_cast<char>(Operation::sub):
                view(lhs, rhs, math::sub(lhs, rhs), Operation::sub);
                break;
            case static_cast<char>(Operation::mul):
                view(lhs, rhs, math::mul(lhs, rhs), Operation::mul);
                break;
            case static_cast<char>(Operation::div):
                if (math::div(lhs, rhs).second) {
                    cout << "Деление на ноль!\n";
                    break;
                }
                view(lhs, rhs, math::div(lhs, rhs).first, Operation::div);
                break;
            case static_cast<char>(Operation::exp):
                view(lhs, rhs, std::pow(lhs, rhs), Operation::exp);
                break;
            default:
                cout << "\nНеизвестная операция, повторите.\n";
                cout << "\nВведите операцию: ";
                continue;
        }
        cout << "\nВведите 'y' для продолжения,\n"
            "или любое другое значение что бы закончить: ";
        
        char q{};
        cin >> q;
        if (q == 'y') {
            cout << "\nВведите операцию: ";
            continue;
        }
        break;
    }
	return 0;
}

void view(double lhs, double rhs, double res, math::Operation op) {
    std::cout << lhs << ' ' << static_cast<char>(op) << ' ' << rhs
        << " = " << res << '\n';
}

template <typename T>
T get_input_value(std::istream& in, const std::string& message)
{
    while (true) {
        T res{}; in >> res;
        if (in.fail()) {
            in.clear();
            in.ignore((std::numeric_limits<std::streamsize>::max)(),
                '\n');
            std::cout << message;
        }
        else {
            in.ignore((std::numeric_limits<std::streamsize>::max)(),
                '\n');
            return res;
        }
    }
}