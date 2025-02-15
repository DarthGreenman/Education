// homework_04_08_01.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "lib/tridiagonal_matrix.h"
#include <memory>
#include <iostream>

int main()
{
    {
        using point = typename tridiagonal_matrix::point;
        auto down = point{ -4.2, 5.5 };
        auto upper = point{ 14.1, 8.2 };
        auto middle = point{ 3.6, 1.8, 7.6 };

        auto matrix = std::make_unique<tridiagonal_matrix>(down, upper, middle);
        auto matrix_clone = matrix->clone();

        std::cout << "Points of the original matrix:\n" << *matrix;
        std::cout << "\n\nPoints of the cloned matrix:\n" << *matrix_clone;
        std::cout << "\n\n";
    }
    
    std::cout << "\n\n";
    std::system("pause");
    return 0;
}