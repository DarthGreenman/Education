// homework_01_05_04.cpp

#include <iostream>
#include <ctime>

#include <Windows.h>

template<typename T>
void swap(T&, T&);

template<size_t N>
void sort_direct_bubble(int(&)[N]);

template<size_t N>
void sort_reverse_bubble(int(&)[N]);

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const size_t kSize{ 10 };
    int array[kSize]{ 0 };

    srand(static_cast<unsigned int>(time(nullptr)));
    for (auto& x : array) {
        x = rand() % 100;
    }

    std::cout << "Массив до сортировки:\t\t";
    for (const auto& x : array) {
        std::cout.width(8);
        std::cout << x;
    }
     
    sort_reverse_bubble<kSize>(array);
    std::cout << "\nМассив после сортировки:\t";
    for (const auto& x : array) {
        std::cout.width(8);
        std::cout << x;
    }
    return 0;
}

template<typename T>
void swap(T& lhv, T& rhv)
{
    T tmp{ lhv };
    lhv = rhv;
    rhv = tmp;
}

template<size_t N>
void sort_direct_bubble(int (&array)[N])
{
#ifdef _DEBUG
    std::pair<size_t, size_t> dim{0, 0};
#endif
    for (size_t f{ 0 }; f < N - 1; ++f) {
        bool is_sorted{ true };
        for (size_t s{ 0 }; s < N - 1 - f; ++s) {
            if (array[s] > array[s + 1]) {
                swap(array[s], array[s + 1]);
                is_sorted = false;
            }
#ifdef _DEBUG
            ++dim.second;
#endif
        }
        if (is_sorted) {
            break;
        }
#ifdef _DEBUG
        ++dim.first;
#endif
    }
#ifdef _DEBUG
    std::cout << "\n\nВнешний цикл: \t\t" << dim.first;
    std::cout << "\nВнутренний цикл: \t" << dim.second << "\n\n";
#endif
}

template<size_t N>
void sort_reverse_bubble(int (&array)[N])
{
#ifdef _DEBUG
    std::pair<size_t, size_t> dim{0, 0};
#endif
    for (size_t f{ 0 }; f < N - 1; ++f) {
        bool is_sorted{ true };
        for (size_t s{ N - 1 }; s > f; --s) {
            if (array[s - 1] > array[s]) {
                swap(array[s - 1], array[s]);
                is_sorted = false;
            }
#ifdef _DEBUG
            ++dim.second;
#endif
        }
        if (is_sorted) {
            break;
        }
#ifdef _DEBUG
        ++dim.first;
#endif
    }
#ifdef _DEBUG
    std::cout << "\n\nВнешний цикл: \t\t" << dim.first;
    std::cout << "\nВнутренний цикл: \t" << dim.second << "\n\n";
#endif

}
