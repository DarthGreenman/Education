// homework_01_12_03.cpp

#include <iostream>
#include <fstream>

#include "..\..\..\..\library\array.h"
#include "..\..\..\..\library\utilities.h"

int main()
{
    setlocale(LC_ALL, "rus");
    using std::cout;
    using std::cin;

    size_t size{};
    cout << "Введите размер массива: "; cin >> size;
    
    seq::Array<int> arr(size);
    for (size_t i{}; i < arr.size(); ++i) {
        cout << "arr[" << i << "] = ";
        int val{};
        cin >> val;
        arr[i] = val;
    }

    seq::reverse(arr.begin(), arr.end()); 
    std::ofstream fout{"out.txt"};
    if(fout.is_open()) {
        fout << arr.size() << '\n';
        for (auto it = arr.cbegin(), end = arr.cend(); it != end; ++it) {
            fout << *it << " " << std::flush;
        }
        fout.close();
    }
    return 0;
}