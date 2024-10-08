// homework_02_06_02.cpp

#include <iostream>
#include <string>
#include <utility>

#include "localisation.h"
#include "counter.h"

bool answer(const std::string&, const std::string&);

template <typename T>
std::istream& get_input_value(std::istream&, T&, const std::string&, 
    const std::string& = "Введите корректное значение для данного типа.");

int main()
{
    const math::Console_localisation local(1251);
    using std::cout;
    using std::cin;
    using std::string;

    math::Counter* count{};
    cout << "Вы хотите указать начальное значение счётчика?"
        "\nВведите 'Да' или 'Нет': ";
    if (const bool assign{ answer("Да", "Нет") }; assign)
    {
        int size{};
        for (;;) {
            if (get_input_value<int>(cin, size,
                "Введите начальное значение счётчика: ") &&
                size >= 0)
            {
                break;
            }
        }
        count = new math::Counter{ static_cast<size_t>(size) };
    }
    else {
        count = new math::Counter{};
    }

    for (char cmd{}; 
        get_input_value<char>(cin, cmd,
            "Введите команду('+', '-', '=' или 'q' для завершения): ") &&
        cmd != 'q';)
    {
        switch (cmd)
        {
            using op = math::Counter::Operation;
        case static_cast<char>(op::inс):
            ++(*count);
            break;
        case static_cast<char>(op::dec):
            --(*count);
            break;
        case static_cast<char>(op::assign):
            cout << "Значение счетчика: "
                << count->get_value()
                << '\n';
            break;
        default:
            cout << "\nНеизвестная команда.\n";
            break;
        }
    }
 
    delete count;
    return 0;
}

bool answer(const std::string& yes, const std::string& no)
{
    std::string answer{};
    for (; std::cin >> answer && answer != yes && answer != no;) {
        std::cout << "Введите 'Да' или 'Нет': ";
    }
    return answer == yes ? true : false;
}

template <typename T>
std::istream& get_input_value(std::istream& in, T& cmd, 
    const std::string& invite, const std::string& alarm)
{
    while (true) {
        std::cout << invite;
        in >> cmd;
        if (in.fail()) {
            in.clear();
            in.ignore((std::numeric_limits<std::streamsize>::max)(),
                '\n');
            std::cout << alarm << '\n';
        }
        else {
            in.ignore((std::numeric_limits<std::streamsize>::max)(),
                '\n');
            return in;
        }
    }
}