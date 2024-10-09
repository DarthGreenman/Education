// homework_01_04_02.cpp

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

    enum class base_number_system {
        Binary = 2, Ternary = 3, Octal = 8,
        Decimal = 10, Duodecimal = 12, Hexadecimal = 16,
        Twenties = 20, Sexagesimal = 60
    };
       
    const unsigned short bns{ 
        static_cast<const unsigned short>(base_number_system::Decimal)
    };
    int sum{ 0 };

    std::cout << "Введите число в диапазоне от " <<
        (std::numeric_limits<int>::min)() << " до " <<
        (std::numeric_limits<int>::max)() << ": ";

    const std::string message{ "\nВы ввели некорректное значение для типа 'int'."
        "\nПовторите ввод, и будьте внимательными: " };
    int number{ 0 };    
    number = get_input_value(message);
    std::cout << "\nВы ввели число: " << number;
   
    while (number != 0) {
            sum += number % bns;
            number /= bns;
    }
    std::cout << "\nСумма цифр: " << sum;
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