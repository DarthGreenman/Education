// Adding to a mutable dynamic array.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
// Добавление в изменяемый динамический массив

#include "my_dynamic_array.h"
#include "my_input.h"
#include "my_localisation.h"

#include <__msvc_string_view.hpp>
#include <cstdlib>
#include <iostream>

namespace my
{
    template<typename T>
    void print_dynamic_array(const My_dynamic_array<T>& array)
    {
        for (std::size_t i{}; i < array.capacity(); ++i)
            i < array.size() ? std::cout << array[i] << ' ' : std::cout << "_ ";
    }

    enum class answer : char { no = 'N', yes = 'Y' };

    auto get_correct_answer(std::string_view message)
    {
        char ret_val{};
        std::cout << message;
        for (; std::cin >> ret_val, toupper(ret_val) != static_cast<char>(answer::yes) &&
            std::toupper(ret_val) != static_cast<char>(answer::no);)
        {
            std::cout << "Повторите ввод, нажмите Y или N:";
        }
        return ret_val;
    }
}

int main()
{
    const my::Console_localisation local{
        my::Console_localisation::Code_pages::rus
    };
    using std::cout;
    if (const auto actual_size = my::get_input_value<std::size_t>("Введите фактичеcкий размер массива: "),
        logical_size = my::get_input_value<std::size_t>("Введите логический размер массива: ");
        logical_size <= actual_size)
    {
        my::My_dynamic_array<int> array(logical_size, actual_size);
        for (std::size_t i{}; i < logical_size; ++i) {
            cout << "Введите [" << i << "]: ";
            const auto value = my::get_input_value<my::My_dynamic_array<int>::value_type>();
            array[i] = value;
        }
        constexpr std::string_view message{ "Динамический массив: " };
        cout << message;
        my::print_dynamic_array(array);

        using std::toupper;
        using answer = typename my::answer;
        for (char go_on{ static_cast<char>(answer::yes) };
            toupper(go_on) == static_cast<char>(answer::yes); )
        {
            cout << "\nВведите значение для добавления: ";
            const auto value = my::get_input_value<my::My_dynamic_array<int>::value_type>();
            array.push_back(value);

            cout << message;
            my::print_dynamic_array(array);

            go_on = my::get_correct_answer("\nДля продолжения добавления нажмите Y,\nдля выхода - N: ");
        }
    }
    else {
        cout << "Ошибка! Логический размер массива не может превышать фактический!\n";
    }

    std::system("pause");
    return 0;
}