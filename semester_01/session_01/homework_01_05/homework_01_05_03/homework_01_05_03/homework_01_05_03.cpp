// homework_01_05_03.cpp

#include <iostream>
#include <limits>

#define NOMINMAX
#include <Windows.h>

struct Position { size_t row; size_t col; };
struct Cell { 
    Position place; 
    int value;
    void set(const size_t& row, const size_t& col, const int& val) {
        place.row = row;
        place.col = col;
        value = val;
    }
};

template<size_t R, size_t C>
const Cell get_max(const int(&)[R][C]);

template<size_t R, size_t C>
const Cell get_min(const int(&)[R][C]);

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const size_t kRow{ 3 },
        kCol{ 6 };
    
    const int matrix[kRow][kCol] {
        1234567, -235657, 123, -687786, 0, -4568,
        0, 12, -1112, 349309, -1255, 0,
        23525, -23435, 64646, 234, -245325, 99999999
    };

    for (const auto& row : matrix) {
        for (const auto& col : row) {
            std::cout.width(10);
            std::cout << col;
        }
        std::cout << '\n';
    }

    const Cell max{ get_max<kRow, kCol>(matrix) };
    const Cell min{ get_min<kRow, kCol>(matrix) };

    std::cout << "\nИндекс минимального элемента: \t" 
        << min.place.row << '\t' << min.place.col;
    std::cout << "\nИндекс максимального элемента: \t" 
        << max.place.row << '\t' << max.place.col;

    return 0;
}


template<size_t R, size_t C>
const Cell get_max(const int (&matrix)[R][C])
{
    Cell max{ { 0, 0 }, std::numeric_limits<int>::min() };
    for (size_t row{ 0 }; row < R; ++row) {
        for (size_t col{ 0 }; col < C; ++col) {
            if (max.value < matrix[row][col]) {
                max.set(row, col, matrix[row][col]);
            }
        }
    }
    return max;
}

template<size_t R, size_t C>
const Cell get_min(const int(&matrix)[R][C])
{
    Cell min{ { 0, 0 }, std::numeric_limits<int>::max() };
    for (size_t row{ 0 }; row < R; ++row) {
        for (size_t col{ 0 }; col < C; ++col) {
            if (min.value > matrix[row][col]) {
                min.set(row, col, matrix[row][col]);
            }
        }
    }
    return min;
}
