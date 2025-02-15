// homework_01_10_03_A.cpp

#include <iostream>

#include "\Users\Darth Greenman\Documents\Visual Studio 2022\source\repos\library\localisation.h"

namespace bag {
    struct dimension {
        size_t rows;
        size_t cols;
    };

    int** create_two_dim_array(const dimension&);
    void fill_two_dim_array(int**, const dimension&);
    void print_two_dim_array(int**, const dimension&);
    void delete_two_dim_array(int**, const dimension&);
}

int main()
{
    bag::localisation();

    using std::cin;
    using std::cout;
        
    cout << "Введите количество строк: ";
    size_t size{ 0 };
    cin >> size;
    
    bag::dimension dim{};
    dim.rows = size;

    cout << "Введите количество столбцов: ";
    cin >> size;
    dim.cols = size;

    try {
        int** mul_table{ bag::create_two_dim_array(dim) };
        bag::fill_two_dim_array(mul_table, dim);
        cout << "Таблица умножения:\n";
        bag::print_two_dim_array(mul_table, dim);
        bag::delete_two_dim_array(mul_table, dim);
    }
    catch (const std::exception& e) {
        cout << e.what();
    }

    return 0;
}

namespace bag {
    int** create_two_dim_array(const dimension& dim) {
        int** matrix = new int* [dim.rows] { 0 };
        for (size_t row{ 0 }; row < dim.rows; ++row) {
            matrix[row] = new int[dim.cols] { 0 };
        }
        return matrix;
    }

    void fill_two_dim_array(int** matrix, const dimension& dim) { 
        for (size_t row{ 0 }; row < dim.rows; ++row) {
            matrix[row][0] = static_cast<int>(row) + 1;
            for (size_t col{ 0 }; col < dim.cols; ++col) {
                matrix[0][col] = static_cast<int>(col) + 1;
                matrix[row][col] = matrix[0][col] * matrix[row][0];
            }
        }
    }
    void print_two_dim_array(int** matrix, const dimension& dim) {
        for (size_t row{ 0 }; row < dim.rows; ++row) {
            for (size_t col{ 0 }; col < dim.cols; ++col) {
                std::cout.width(5);
                std::cout << matrix[row][col];
            }
            std::cout << '\n';
        }
    }
    void delete_two_dim_array(int** matrix, const dimension& dim) {
        for (size_t row{ 0 }; row < dim.rows; ++row) {
            delete[] matrix[row];
        }
        delete[] matrix;
        matrix = nullptr;
    }
}