// homework_04_07_01.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "lib/my_algorithms.h"
#include "lib/my_vector.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>


int main()
{
    seq::My_vector<int> vec_i{ -2,0,1,0,1,-23,0,1,1,1,1,2,4,5,12 };
    
    std::cout << "Original: ";
    std::for_each(std::cbegin(vec_i), std::cend(vec_i), 
        [](int Elem) ->void { std::cout << Elem << ' '; });
    std::cout << "\nsize: " << vec_i.size() << '\n' << "capacity: " << vec_i.capacity();
    
    // Удаляем дубликаты при создании нового вектора (т. к. функции erase пока нет), 
    // но прежде сортируем
    seq::sort::count(std::begin(vec_i), std::end(vec_i));

#ifdef DEBUG
    std::cout << "\n\nSorted: ";
    std::for_each(std::cbegin(vec_i), std::cend(vec_i),
        [](Type Elem) ->void { std::cout << Elem << ' '; });
    std::cout << "\nsize: " << vec_i.size() << '\n' << "capacity: " << vec_i.capacity();
#endif // DEBUG

    const auto vec_o{
        seq::make_unique_vector(std::begin(vec_i), std::end(vec_i))
    };
    
    std::cout << "\n\nUnique: ";
    std::for_each(std::begin(vec_o), std::end(vec_o), 
        [](int Elem) ->void { std::cout << Elem << ' '; });
    std::cout << "\nsize: " << vec_o.size() << '\n' << "capacity: " << vec_o.capacity();
    
    std::cout << "\n\n";
    std::system("pause");
    return 0;
}