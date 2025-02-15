// homework_04_09_02.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "../lib/my_bcd.h"
#include "../lib/my_numeric.h"
#include "../lib/my_ultra_long.h"

#include <cstdlib>
#include <iomanip>
#include <iostream>


int main()
{
    ///*
    using std::cout;
    using std::setw;
    using packed = numbers::packed_t;
    using unpacked = numbers::unpacked_t;
    using numbers::to_string_view;

    numbers::Ultra_long<32, packed> num_1{ "9235" };
    numbers::Ultra_long<32, packed> num_2{ "1538" };
    cout << num_1 << setw(10) << to_string_view(num_1) << '\n';
    cout << num_2 << setw(10) << to_string_view(num_2) << '\n';
  
    num_1 += num_2;
    cout << num_1 << setw(10) << to_string_view(num_1) << '\n';
    cout << "\n\n";
      
    const numbers::Ultra_long<32, packed> num_3{ "9236" };
    const numbers::Ultra_long<32, packed> num_4{ "1538" };
    cout << num_3 << setw(10) << to_string_view(num_3) << '\n';
    cout << num_4 << setw(10) << to_string_view(num_4) << '\n';

    const auto num_5 = num_3 + num_4;
    cout << (num_3 + num_4) << setw(10) << to_string_view(num_5) << '\n';
    cout << "\n\n";

    num_1 += num_5;
    cout << num_1 << setw(10) << to_string_view(num_1) << '\n';

    //*/
    std::system("pause");
    return 0;
}