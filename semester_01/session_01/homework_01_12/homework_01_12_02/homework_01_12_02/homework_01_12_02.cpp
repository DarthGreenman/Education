// homework_01_12_02.cpp
#include <iostream>
#include <fstream>


#include "..\..\..\..\library\array.h"
#include "..\..\..\..\library\utilities.h"

int main()
{
    setlocale(LC_ALL, "Russian");
    using std::cout;
    
    std::ifstream fin("in.txt");
    if (!fin.is_open()) {
        cout << "Файл не может быть открыт!\n";
        return 1;
    }

    // Чтение и запись
    size_t size{}; 
    fin >> size;
    seq::Array<int> arr(size);
    int x{};
    for (size_t i{}; i < size && fin >> x; ++i) {
        arr[i] = x;
    }
    fin.close();
        
    cout << "Исходный массив:\n";
    for (auto beg = arr.cbegin(), end = arr.cend(); beg != end; ++beg) {
        cout << *beg << " ";
    }
    
    // Операции с массивом
    cout << "\n\nМассив с обратной последовательнотью:\n";
    seq::reverse(arr.begin(), arr.end());
    for (auto beg = arr.cbegin(), end = arr.cend(); beg != end; ++beg) {
        cout << *beg << " ";
    }
    cout << "\n\nОтсортированный массив:\n";
    seq::sort(arr.begin(), arr.end());
    for (auto beg = arr.cbegin(), end = arr.cend(); beg != end; ++beg) {
        cout << *beg << " ";
    }
    cout << "\n\nСумма элементов массива: " << seq::sum(arr.cbegin(), arr.cend());

    return 0;
}