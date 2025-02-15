// homework_03_02_01.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <chrono>

#include "Fibonacci.h"

int main()
{
    constexpr size_t num{ 40 };

    for (size_t i{}; i <= num; ++i)
    {
        const auto begin = std::chrono::system_clock::now();
        const size_t f{
            seq::Fibonacci::recursive_method(i)
        };
        const auto end = std::chrono::system_clock::now();
        std::cout << i << ":\t" << f << '\t'
            << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n";
    }
    
    return 0;
}