// homework_04_05_02.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "lib/matrix_alt.h"
#include "lib/matrix_seq.h"

#include <iostream>
#include <iomanip>
#include <cassert>

int main()
{
    constexpr size_t Row{ 5 };
    constexpr size_t Col{ 10 };

    {
        seq::Matrix<int> table(Row, Col);
        const auto&& [row, col] = table.size();
        for (size_t i{}; i < row; ++i) {
            for (size_t j{}; j < col; ++j) {
                table[i][j] = static_cast<int>(i * j);
            }
        }
        std::cout << "template<typename T>\nclass seq::Matrix\n";
        for (size_t i{}; i < row; ++i) {
            for (size_t j{}; j < col; ++j) {
                assert(table[i][j] == static_cast<int>(i * j));
                std::cout << std::setw(5) << table[i][j];
            }
            std::cout << '\n';
        }
        std::cout << "Array dimensionality:";
        std::cout << "\nrow count:\t" << row;
        std::cout << "\ncolumn count:\t" << col;
        constexpr size_t offset{ 1 };
        std::cout << '\n' << "table[" << row - offset << "][" << col - offset << "]: "
            << table[row - offset][col - offset] << '\n';
    }

    {
        alternative::Matrix<int, Row, Col> table;
        const auto&& [row, col] = table.size();
        for (size_t i{}; i < row; ++i) {
            for (size_t j{}; j < col; ++j) {
                table(i, j) = static_cast<int>(i * j);
            }
        }
        std::cout << "\ntemplate<typename T, size_t Row, size_t Col>"
            "\nclass alternative::Matrix\n";
        for (size_t i{}; i < row; ++i) {
            for (size_t j{}; j < col; ++j) {
                assert(table(i, j) == static_cast<int>(i * j));
                std::cout << std::setw(5) << table(i, j);
            }
            std::cout << '\n';
        }
        std::cout << "Array dimensionality:";
        std::cout << "\nrow count:\t" << row;
        std::cout << "\ncolumn count:\t" << col;
        constexpr size_t offset{ 2 };
        std::cout << '\n' << "table[" << row - offset << "][" << col - offset << "]: "
            << table(row - offset, col - offset) << '\n';
    }
  
    std::system("pause");
    return 0;
} 