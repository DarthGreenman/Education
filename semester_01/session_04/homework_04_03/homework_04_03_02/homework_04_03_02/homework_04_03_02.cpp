// homework_04_03_02.cpp
/* В этом задании вам нужно поработать с классом умных массивов, 
 * который вы реализовали в предыдущем задании. Сначала попробуйте создать
 * два экземпляра вашего класса с различными элементами и присвоить один другому.
 */

#include "array.h"
#include <iostream>

int main()
{
    const seq::Array<int> numbers_s{ 1,3,8,0,-1,4 };
    std::cout << "numbers_s: ";
    std::for_each(numbers_s.begin(), numbers_s.end(),
        [](const int x) {std::cout << x << ' '; });
    
    seq::Array<int> numbers_t;
    numbers_t.push_back(0);
    numbers_t.push_back(-100);
    std::cout << "\nnumbers_t: ";
    std::for_each(numbers_t.begin(), numbers_t.end(),
        [](const int x) {std::cout << x << ' '; });

    numbers_t = numbers_s;
    std::cout << "\nnumbers_t = numbers_s: ";
    for (const auto& x : numbers_t) {
        std::cout << x << ' ';
    }

    std::cout << '\n';
    const seq::Array<int> numbers(numbers_s.cbegin() + 1, numbers_s.cend() - 1);
    for (const auto& x : numbers) {
        std::cout << x << ' ';
    }

    return 0;
}