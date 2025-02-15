// homework_01_05_02.cpp

#include <iostream>
#include <limits>

#define NOMINMAX
#include <Windows.h>

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const int array[] = { 1234567, -235657, 123, -687786, 0,
        -4568, 798787, 0, 12, -11123216
    };
  
    int min{ std::numeric_limits<int>::max() };
    int max{ std::numeric_limits<int>::min() };
    for (const int& x : array) {
        min = min > x ? x : min;
        max = max < x ? x : max;   
        std::cout << x << " ";
    }
    std::cout << "\n\nМинимальное значение: \t" << min
        << "\nМаксимальное значение: \t" << max;
    return 0;
}
