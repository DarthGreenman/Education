// homework_01_12_04.cpp

#include <iostream>
#include <fstream>

#include "..\..\..\..\library\array.h"
#include "..\..\..\..\library\utilities.h"

int main()
{
    setlocale(LC_ALL, "rus");
    using std::cout;

    std::ifstream fin("in.txt");
    if (!fin.is_open()) {
        cout << "Файл не может быть открыт!\n";
        return 1;
    }
    
    // Чтение и запись
    size_t row{}, col{};
    fin >> row >> col;
    seq::Array<seq::Array<int>> mtx(row, col);
    for (auto row = mtx.begin(); row != mtx.cend(); ++row) {
        int val{};
        for (auto col = row->begin(); col != row->cend() &&
            fin >> val; ++col) {
            *col = val;
        }
    }
    fin.close();
    
    cout << "Исходный массив : \n";
    for (auto row = mtx.cbegin(), end = mtx.cend(); row != end; ++row) {
        for (auto col = row->cbegin(), end = row->cend(); col != end; ++col) {
            cout << *col << " ";
        }
        cout << '\n';
    }
    // Реверс значений в строках
    for (auto row = mtx.cbegin(), end = mtx.cend(); row != end; ++row) {
        seq::reverse(row->begin(), row->end());
    }
    cout << "\n\nМассив с обратной последовательностью элементов в строках: \n";
    for (auto row = mtx.cbegin(), end = mtx.cend(); row != end; ++row) {
        for (auto col = row->cbegin(), end = row->cend(); col != end; ++col) {
            cout << *col << " ";
        }
        cout << '\n';
    }
    return 0;
}