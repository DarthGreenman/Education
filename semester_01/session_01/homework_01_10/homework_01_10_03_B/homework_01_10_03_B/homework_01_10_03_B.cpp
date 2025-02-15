// homework_01_10_03_B.cpp

#include <iostream>

#include "..\..\..\..\..\..\library\localisation.h"

namespace bag {
    class mul_table {
    public:
        mul_table() = delete;
        mul_table(size_t, size_t);
        ~mul_table();

        void print() const;

    private:
        void fill();

    private:
        struct dimension {
            size_t rows;
            size_t cols;
        };
        dimension dim_;
        int** data_;
    };
}

int main()
{
    bag::localisation();

    using std::cin;
    using std::cout;

    std::pair<char, char> ans{'y', 'n'};

    for (char key{ ans.first }; key == ans.first;) {
        cout << "Введите количество строк: ";
        size_t rows{ 0 };
        cin >> rows;

        cout << "Введите количество столбцов: ";
        size_t cols{ 0 };
        cin >> cols;

        try {
            bag::mul_table table{rows, cols};
            cout << "Таблица умножения:\n";
            table.print();

            cout << "Хотите продолжить: нажмите 'y',\n";
            cout << "для окончания нажмите 'n': ";
            do {
                cin >> key;
            } while (key != ans.first && key != ans.second);
        }
        catch (const std::exception& e) {
            cout << e.what() << '\n';
        }
    }

    return 0;
}

namespace bag{
    mul_table::mul_table(size_t rows, size_t cols) {
        dim_.rows = rows;
        dim_.cols = cols;
        data_ = new int* [dim_.rows] {};
        for (size_t row{ 0 }; row < dim_.rows; ++row) {
            data_[row] = new int[dim_.cols] {};
        }
        fill();
    }

    mul_table::~mul_table() {
        for (size_t row{ 0 }; row < dim_.rows; ++row) {
            delete[] data_[row];
        }
        delete[] data_;
        data_ = nullptr;
    }

    void mul_table::fill() {
        for (size_t row{ 0 }; row < dim_.rows; ++row) {
            data_[row][0] = static_cast<int>(row) + 1;
            for (size_t col{ 0 }; col < dim_.cols; ++col) {
                data_[0][col] = static_cast<int>(col) + 1;
                data_[row][col] = data_[0][col] * data_[row][0];
            }
        }
    }

    void mul_table::print() const {
        for (size_t row{ 0 }; row < dim_.rows; ++row) {
            for (size_t col{ 0 }; col < dim_.cols; ++col) {
                std::cout.width(5);
                std::cout << data_[row][col];
            }
            std::cout << '\n';
        }
    }
}