// homework_01_06_03.cpp

#include <iostream>
#include <vector>

#include <Windows.h>

const unsigned Fibonacci(unsigned);
unsigned get_input_value(const std::string&);


int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const std::string message{ "\nВы ввели некорректное значение для типа 'int'."
        "\nПовторите ввод, и будьте внимательными: " };

    std::cout << "Введите число: ";
    unsigned number{ get_input_value(message) };
    
    std::vector<unsigned> Fibonacci_numbers;
    for (unsigned i{ 0 }; i < number; ++i) {
        Fibonacci_numbers.push_back(Fibonacci(i));
    }

    std::cout << "\nЧисла Фибоначчи: ";
    for (const unsigned& n : Fibonacci_numbers) {
        std::cout << n << ' ';
    }   
    
    return 0;
}

const unsigned Fibonacci(unsigned number)
{
    return number == 0 || number == 1 ? 
        number : Fibonacci(number - 2) + Fibonacci(number - 1);
}

unsigned get_input_value(
    const std::string& message
)
{
    while (true) {
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
}