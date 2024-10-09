// homework_01_09_01.cpp

#include <iostream>

namespace bag {
    void print(const int[], const size_t);
    void print(const double[], const size_t);
}

int main()
{
    using std::cout;
    
    int arr1[]{ 1, 2, -6, 124, 0, 765, 1098 };
    cout << "Массив { 1, 2, -6, 124, 0, 765, 1098 }\t: ";
    bag::print(&arr1[0], sizeof(arr1) / sizeof(arr1[0]));

    const size_t N{ 4 };
    int arr2[4]{ 345, 0, 4, 5567 };
    cout << "\nМассив { 345, 0, 4, 5567 }\t\t: ";
    bag::print(arr2, N);

    double arr3[]{ 0.45, 78.095, 356.789 };
    cout << "\nМассив { 0.45, 78.095, 356.789 }\t: ";
    bag::print(arr3, sizeof(arr3) / sizeof(arr3[0]));
    
    return 0;
}

namespace bag {
    void print(const int array[], const size_t size) {
        for (size_t i{ 0 }; i < size; ++i) {
            std::cout << array[i] << ' ';
        }
    }

    void print(const double array[], const size_t size) {
        for (size_t i{ 0 }; i < size; ++i) {
            std::cout << array[i] << ' ';
        }
    }
}
