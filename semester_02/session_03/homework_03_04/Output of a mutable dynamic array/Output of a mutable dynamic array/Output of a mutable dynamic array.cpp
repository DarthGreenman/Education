// Output of a mutable dynamic array.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
// Вывод изменяемого динамического массива

#include "my_dynamic_array.h"
#include "my_input.h"
#include "my_localisation.h"

#include <cstdlib>
#include <iostream>
#include <memory>

namespace my
{
    template<typename T>
    void print_dynamic_array(T* array, std::size_t logical_size, std::size_t actual_size)
    {
        for (std::size_t i{}; i < actual_size; ++i)
            i < logical_size ? std::cout << array[i] << ' ' : std::cout << "_ ";
    }

    template<typename T>
    void print_dynamic_array(const My_dynamic_array<T>& array, std::size_t logical_size)
    {
        for (std::size_t i{}; i < array.size(); ++i)
            i < logical_size ? std::cout << array[i] << ' ' : std::cout << "_ ";
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
        my::My_dynamic_array<int> array(actual_size);
        for (std::size_t i{}; i < logical_size; ++i) {
            cout << "Введите [" << i << "]: ";
            const auto value = my::get_input_value<int>();
            array[i] = value;
        }

        cout << "Динамический массив: ";
        my::print_dynamic_array(array, logical_size);
        cout << '\n';
    }
    else {
        cout << "Ошибка! Логический размер массива не может превышать фактический!\n";
    }

    std::system("pause");
    return 0;
}
