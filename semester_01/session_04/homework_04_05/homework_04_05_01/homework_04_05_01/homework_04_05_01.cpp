// homework_04_05_01.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "lib/square.h"
#include <iostream>
#include <vector>

int main()
{   
    using std::cout;
    /* Возведение в квадрат числа */
    constexpr double number{ 123.0025 };
    cout << "[IN]:\t" << number;
    cout << "\n[OUT]:\t" << seq::square(number);
    
    /* Возведение в квадрат значений элементов */
    const std::vector<int> vec_s{ 1,4,-100,345,213 };
    cout << "\n[IN]:\t";
    std::for_each(vec_s.cbegin(), vec_s.cend(), [&vec_s](int Number) {
        static size_t count{};
        cout << Number << (count++ != (vec_s.size() - 1) ? ", " : "");
    });

    const auto vec_t = seq::square(vec_s);
    cout << "\n[OUT]:\t";
    std::for_each(vec_t.cbegin(), vec_t.cend(), [&vec_t](int Number) {
        static size_t count{};
        cout << Number << (count++ != (vec_t.size() - 1) ? ", " : "");
    });
    cout << '\n';
    std::system("pause");
    return 0;
}