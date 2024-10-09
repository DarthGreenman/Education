// explicit_template_instantiation.cpp : Этот файл содержит функцию "main".
//

/* ВАРИАНТ - 1
 * Явное инстанцирование (конкретизация) с помощью резервированного слова 
 * template (см. utilities_example_01.cpp) */
#include "utilities_example_01.h"

/* ВАРИАНТ - 2 предложенный Страуструпом */
#include "utilities_example_02.h"
#include "utilities_example_02.cpp"

#include <iostream>

int main()
{
    std::cout << "min: " << seq::min(100, 200) << '\n';
    std::cout << "min: " << seq::min(279.67, -345.67) << '\n';

    std::cout << "max: " << seq::max(100, 200) << '\n';
    std::cout << "max: " << seq::max(279.67, -345.67) << '\n';

    return 0;
}