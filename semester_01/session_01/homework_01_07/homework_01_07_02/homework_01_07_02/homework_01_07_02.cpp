// homework_01_07_02.cpp

#include <iostream>
#include <cmath>

namespace use {
    int move(int);
    void swap(int&, int&);
}

namespace not_use{
    /* ОГРАНИЧЕНИЯ:
       1. Значения параметров типов int должны быть беззнаковыми.
       2. Второе ограничение на длину (разрядность) значений параметров.
          Точные границы привести затрудняюсь.
    */ 
    void swap(int&, int&);
    const int get_digits_number(int);
    const int get_base_decimal_number();
}

int main()
{
    using std::cout;
    int a{ 9000 };
    int b{ 124 };

    cout << "Начальные условия:\n";
    cout << "a = " << a << ", b = " << b;

    not_use::swap(a, b);
    cout << "\n\nОтработала функця 'not_use::swap(a, b)':\n";
    cout << "a = " << a << ", b = " << b;

    use::swap(a, b);
    cout << "\n\nОтработала функця 'use::swap(a, b)':\n";
    cout << "a = " << a << ", b = " << b;


    return 0;
 }

int use::move(int a) {
    int* b{ &a };
    return *b;
}

void use::swap(int& a, int& b) {
    const int tmp{ use::move(a) };
    a = use::move(b);
    b = use::move(tmp);
}

void not_use::swap(int& a, int& b)
{
    using not_use::get_digits_number;
    using not_use::get_base_decimal_number;
    using std::pow;
 
    const int number_digit_a{ get_digits_number(a) };
    const int number_digit_b{ get_digits_number(b) };
    const int number_base{ get_base_decimal_number() };

    // Помещаем в 'b' значение 'a' предварительно увеличив
    // 'a' на количество разрядов 'b'. 
    // a{2000}; b{100}; a *= 10^2; b += a;
    // теперь 'b' имеет значение 2000100
    a *= static_cast<int>(pow(number_base, number_digit_b));
    b += a;
    a = 0;
    for (int i{ 0 }; i < number_digit_b; ++i) {
        a += b % number_base * static_cast<int>(pow(number_base, i));
        b /= number_base;
    }
}

const int not_use::get_digits_number(int number)
{
    int res{ 0 };
    for (; number != 0; ++res) {
        number /= not_use::get_base_decimal_number();
    }
    return res;
}

const int not_use::get_base_decimal_number()
{
    return 10;
}