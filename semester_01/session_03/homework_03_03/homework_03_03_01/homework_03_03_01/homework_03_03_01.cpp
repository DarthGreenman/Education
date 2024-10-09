// homework_03_03_01.cpp
/*
 * Нужно написать функцию с сигнатурой void merge_sort(int* arr, int size),
 * которая принимает на вход массив целых чисел и его размер.
 * Функция должна отсортировать полученный массив методом слияния.
 * 
 * Для проверки используйте массивы:
 * 3 43 38 29 18 72 57 61 2 33;
 * 88 91 87 59 53 49 29 16 4 27 28 89 2 25 74;
 * 24 66 20 79 30 16 19 62 94 59 0 7 59 90 84 60 95 62. 
*/

#include <iostream>
#include <vector>
#include <algorithm>

#include "sort.h"
#include "localisation.h"

template<typename T>
void view(const std::vector<T> vec);

template<typename T, size_t N>
void view(T(&arr)[N]);

int main()
{
    const bag::Console_localisation locale{
        bag::Console_localisation::Code_pages::rus
    };
    
    std::cout << "Исходный массив: ";
    int vec1[]{ 3,43,38,29,18,72,57,61,2,33 };
    view(vec1);
    std::cout << "Отсортированный массив: ";
    seq::sort::quick(std::begin(vec1), std::end(vec1));
    view(vec1);

    std::cout << "\nИсходный массив: ";
    std::vector<double> vec2{ 24.6,66.9,20.0,79,30.1,16,19,62,94,59,0,7,59,90,84,60,95,62 };
    view(vec2);
    std::cout << "Отсортированный массив: ";
    seq::sort::quick(vec2);
    view(vec2);

    std::cout << "\nИсходный массив: ";
    int arr[]{ 24,66,20,79,30,16,-19,-19,62,94,59,0,7,59,90,84,60,95,-62 };
    view(arr);
    std::cout << "Отсортированный массив: ";
    seq::sort::quick(arr, std::size(arr));
    view(arr);

    std::system("pause");
    return 0;
}

template<typename T>
void view(const std::vector<T> vec)
{
    for (const auto v : vec) {
        std::cout << v << ' ';
    }
    std::cout << '\n';
}

template<typename T, size_t N>
void view(T(&arr)[N])
{
    for (T* p{ arr }; p != arr + N; ++p) {
        std::cout << *p << ' ';
    }
    std::cout << '\n';
}