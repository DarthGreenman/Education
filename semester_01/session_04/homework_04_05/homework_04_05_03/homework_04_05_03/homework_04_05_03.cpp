// homework_04_05_03.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "lib/functors.h"

#include <iostream>
#include <vector>
#include <type_traits>
#include <string_view>

namespace helper
{
    template<typename T, size_t N>
        requires seq::Appropriate_type<T>
    void print(const T(&Array)[N]);

    template<typename InputIt>
    void print(InputIt First, InputIt Last);
}
int main()
{
    constexpr unsigned divisor{ 3 };
    const std::string_view message_sum{ "\nSum of items: " };
    const std::string_view message_mul{ "\nNumber of elements divisible by "};
    
    /* Массив */
    constexpr int arr[]{ 4,1,3,6,25,54 };
    const seq::adder_element<int> sum_arr(arr);
    const seq::multiple_elements mul_arr(arr);
    
    std::cout << "C-style array: "; helper::print(arr);
    std::cout << message_sum << sum_arr();
    std::cout << message_mul << divisor << ": " << mul_arr(divisor);

    /* Список */
    const seq::adder_element<double> sum_list(
        { 4.1,1.1,3.1,6.1,25.1,54.1 }
    );
    std::cout << "\n\nInitialisation list: 4.1, 1.1, 3.1, 6.1, 25.1, 54.1";
    std::cout << message_sum << sum_list();

    /* Вектор */
    const std::vector<int> vecc{ -123,-122,-121,-120,0,120,121,122,123 };
    const seq::adder_element<int> sum_vecc(vecc);
    const seq::multiple_elements mul_vecc(vecc);

    std::cout << "\n\nVector: "; helper::print(vecc.cbegin(), vecc.cend());
    std::cout << message_sum << sum_vecc();
    std::cout << message_mul << divisor << ": " << mul_vecc(divisor);

    /* Вектор */
    std::vector<double> vecm{ 4.5,1.5,3.5,6.5,25.5,54.0 };
    std::cout << "\n\nVector: "; helper::print(vecm.cbegin(), vecm.cend());
    
    const seq::adder_element<double> sum_vecm(std::move(vecm));
    std::cout << message_sum << sum_vecm();
    
    /* Диапазон */
    const seq::adder_element<double> sum_vecr(
        std::begin(vecc) + 2, std::end(vecc) - 2
    );
    const seq::multiple_elements mul_vecr(
        std::begin(vecc) + 2, std::end(vecc) - 2
    );
    std::cout << "\n\nRange: "; helper::print(
        std::begin(vecc) + 2, std::end(vecc) - 2
    );
    std::cout << message_sum << sum_vecr();
    std::cout << message_mul << divisor << ": " << mul_vecr(divisor);
    
    std::cout << '\n';
    std::system("pause");
    return 0;
}

namespace helper
{
    template<typename T, size_t N> 
        requires seq::Appropriate_type<T>
    void print(const T(&Array)[N])
    {
        for (size_t i{}; i < N; ++i) {
            const char* sep{ i != N - 1 ? ", " : "." };
            std::cout << Array[i] << sep;
        }
    }

    template<typename InputIt>
    void print(InputIt First, InputIt Last)
    {
        while (First != Last) {
            const char* sep{ First != Last - 1 ? ", " : "." };
            std::cout << *First << sep;
            ++First;
        }
    }
}