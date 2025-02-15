// homework_03_02_02.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream> 
#include <chrono>

#include "Fibonacci.h"

int main()
{
    const size_t num{ 40 };

    auto begin = std::chrono::system_clock::now();
    const size_t f1{
        seq::Fibonacci::recursive_method(num)
    };
    auto end = std::chrono::system_clock::now();
    std::cout << f1 << ":\t"
        << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n";

    begin = std::chrono::system_clock::now();
    const size_t f2{
        seq::Fibonacci::recursive_memo_method(num)
    };
    end = std::chrono::system_clock::now();
    std::cout << f2 << ":\t"
        << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n";

    return 0;
}