// homework_01_10_02_B.cpp

#include <iostream>

#include "\Users\Darth Greenman\Documents\Visual Studio 2022\source\repos\library\localisation.h"


namespace bag {
    void print(const int[], const size_t);
    void print(const double[], const size_t);

    double* create_array(const size_t);
    void destroy_array(double*);
}


int main()
{
    bag::localisation();

    using std::cin;
    using std::cout;

    cout << "Введите размер массива: ";
    size_t size{ 0 };
    cin >> size;

    double* p_arr{ bag::create_array(size) };
    if (!p_arr) {
        cout << "Недостаточно памяти.";
    }
    else {
        cout << "Массив: ";
        bag::print(p_arr, size);
        bag::destroy_array(p_arr);
    }

    return 0;
}

namespace bag {
    void print(const int array[], const size_t size) {
        for (size_t i{ 0 }; i < size; ++i) {
            std::cout << array[i] << ' ';
        }
    }

    void print(const double array[], const size_t size) {
        for (size_t i{ 0 }; i < size; ++i) {
            std::cout << array[i] << ' ';
        }
    }

    double* create_array(const size_t size) {
        return new(std::nothrow) double[size] {};
    }
    void destroy_array(double* array) {
        delete[] array;
        array = nullptr;
    }
}