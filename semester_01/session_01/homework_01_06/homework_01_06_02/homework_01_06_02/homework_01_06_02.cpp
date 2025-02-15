// homework_01_06_02.cpp

#include <iostream>

int exponentiate(int base, int exponent);

int main()
{
    const int b1{ 5 }, e1{ 2 },
        b2{ 3 }, e2{ 3 },
        b3{ 4 }, e3{ 4 };

    std::cout << b1 << " в степени " << e1 << " = " << exponentiate(b1, e1) << '\n';
    std::cout << b2 << " в степени " << e2 << " = " << exponentiate(b2, e2) << '\n';
    std::cout << b3 << " в степени " << e3 << " = " << exponentiate(b3, e3);

    return 0;
}

int exponentiate(int base, int exponent)
{
    int result{ 1 };
    for (size_t i{ 0 }; i < exponent; ++i) {
        result *= base;
    }
    return result;
}
