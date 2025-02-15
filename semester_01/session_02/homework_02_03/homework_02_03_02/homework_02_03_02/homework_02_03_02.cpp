// homework_02_03_02.cpp 

#include <iostream>
#include <string>
#include <utility>

#include "localisation.h"
#include "counter.h"

const bool answer(const std::string&, const std::string&);

template <typename T>
T get_input_value(const std::string & =
    "Введите правильное цифровое значение: "
);

int main()
{
    const math::console_localisation localisation(1251);
    using std::cout;
    using std::cin;
    using std::string;

    math::Counter* count{};
    cout << "Вы хотите указать начальное значение счётчика?"
        "\nВведите 'Да' или 'Нет': ";
    if (const bool assign_value{ answer("Да", "Нет") };
        assign_value == true)
    {
        cout << "Введите начальное значение счётчика: ";
        const size_t value{get_input_value<size_t>()};
        count = new math::Counter{ value };
    }
    else {
        count = new math::Counter{};
    }

    for (;;) {
        cout << "Введите команду('+', '-', '=' или 'Q' для завершения): ";
        char command_id{};
        cin >> command_id;
        auto&& [repeat, view] = count->repeat_command(command_id);
        if (!repeat) {
            cout << "До свидания!";
            break;
        }
        if (view) {
            cout << "Значение счетчика: " << count->get_value()
                << '\n';
        }
    }
    
    delete count;
    return 0;
}

const bool answer(const std::string& yes, const std::string& no)
{
    std::string answer{};    
    for (; std::cin >> answer && answer != yes && answer != no;) {
        std::cout << "Введите 'Да' или 'Нет': ";
    }
    return answer == yes ? true : false;
}

template <typename T>
T get_input_value(
    const std::string& message
)
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
