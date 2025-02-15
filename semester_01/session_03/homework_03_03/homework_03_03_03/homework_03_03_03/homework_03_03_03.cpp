// homework_03_03_03.cpp
/*
 * Нужно написать функцию с сигнатурой void count_sort(int* arr, int size), 
 * которая принимает на вход массив целых чисел и его размер.
 * Функция должна отсортировать полученный массив методом «сортировка подсчётом».
 * Известно, что массив, который необходимо отсортировать, состоит из целых чисел
 * не меньше 10 и не больше 24.

 * Для проверки используйте массивы:
 * 19 14 22 22 17 22 13 21 20 24 18 10 17 16 17 20 22 11 20 16 14 13 10 22 18 14 16 24 19 17;
 * 16 17 14 20 22 20 17 22 16 19 23 24 20 22 21 18 14 16 17 21 10 11 19 23 11 11 17 17 11 21 17 11 17 16 12 11 16 22 23 16;
 * 21 15 19 18 23 12 18 18 19 23 12 20 15 22 21 18 19 20 12 16 20 14 17 13 10 23 19 14 10 22 19 12 24 23 22 15 13 22 18 18 11 23 24 17 10.
*/

#include <iostream>
#include <utility>

#include "localisation.h"
#include "sort.h"

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
    std::vector<int> arr1{ 19,14,22,22,-17,22,13,-21,20,24,18,10,17,16,17,20,22,11,20,16,14,13,
       10,22,18,14,16,24,19,17,0 };
    view(arr1);
    std::cout << "Отсортированный массив: ";
    seq::sort::count(arr1);
    view(arr1);

    
    std::cout << "\nИсходный массив: ";
    int arr2[]{ 16,17,14,20,22,20,17,22,16,19,-23,24,20,22,21,18,14,16,17,21,10,11,
        19,23,11,11,17,17,11,21,17,0,11,17,16,12,11,16,22,23,-16 };
    view(arr2);
    std::cout << "Отсортированный массив: ";
    seq::sort::count(arr2, std::size(arr2));
    view(arr2);
    
    std::cout << "\nИсходный массив: ";
    int arr3[]{ 21,15,0,19,18,-23,12,-18,18,19,23,-12,20,15,22,21,18,19,20,12,16,20,14,
        17,13,10,23,19,14,10,0,-22,19,-12,24,23,22,-15,13,22,18,18,11,23,24,17,10 };
    view(arr3);
    std::cout << "Отсортированный массив: ";
    seq::sort::count(std::begin(arr3), std::end(arr3));
    view(arr3);
    
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