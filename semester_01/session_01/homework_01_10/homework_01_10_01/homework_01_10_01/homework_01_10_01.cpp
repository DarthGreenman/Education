// homework_01_10_01.cpp 

#include <iostream>

#include "\Users\Darth Greenman\Documents\Visual Studio 2022\source\repos\library\localisation.h"


namespace bag {
    void print(const int[], const size_t);
    void print(const double[], const size_t);
}


int main()
{
    bag::localisation();

    using std::cin;
    using std::cout;

    cout << "Введите размер массива: ";
    size_t size{ 0 };
    cin >> size;

    try {
        int* p_arr{ new int[size] {} };
        for (size_t i{ 0 }; i < size; ++i) {
            cout << "arr[" << i << "] = "; cin >> p_arr[i];
        }

        cout << "Введённый массив: ";
        bag::print(p_arr, size);

        delete[] p_arr;
        p_arr = nullptr;
    }
    catch (const std::exception& e) {
        cout << e.what();
    }
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
}
