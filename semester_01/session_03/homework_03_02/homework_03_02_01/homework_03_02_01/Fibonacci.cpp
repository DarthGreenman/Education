/* Fibonacci.cpp */

#include <vector>

#include "Fibonacci.h"

namespace seq
{
    size_t Fibonacci::iterative_method(size_t num)
    {
        if (num == 0 || num == 1) {
            return num;
        }

        size_t f{};
        for (size_t i{ 2 }, a{}, b{ 1 }; i <= num; ++i)
        {
            f = a + b;
            a = b;
            b = f;
        }
        return f;
    }

    size_t Fibonacci::recursive_method(size_t num)
    {
        if (num == 0 || num == 1) {
            return num;
        }
        return Fibonacci::recursive_method(num - 2) +
            Fibonacci::recursive_method(num - 1);
    }
}
