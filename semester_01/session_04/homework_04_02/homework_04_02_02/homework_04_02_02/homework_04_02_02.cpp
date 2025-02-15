// homework_04_02_02.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <memory>

/*
* ОШИБКИ СИНТАКСИЧЕСКИЕ:
* 1. Отсутствующая парная скобка.
* 2. "." вместо "," и наоборот.
* 3. "1" вместо "l" и наоборот.
* 4. Отсутствует ";".
* 5. Ошибка в написании ключевых слов.
*/

/*
* ОШИБКИ СЕМАНТИЧЕСКИЕ (В КОДЕ).
*/

/*
* ОШИБКИ ЛИНКОВКИ:
* На мой взгляд одна из самых распространенных ошибок - ошибка при связывании
* имен функций, ШАБЛОНОВ.
*/


#define _SEMANTICALLY_INCORRECT_CODE

/*
* Возврат адреса локальной переменной функции
*/
namespace axis
{
    using abscissa = double;
    using ordinate = double;
    using applicata = double;
}

struct Point {
    axis::abscissa x{};
    axis::ordinate y{};
    axis::applicata z{};
};

#ifdef SEMANTICALLY_INCORRECT_CODE
Point& get_point(axis::abscissa x, axis::ordinate y, axis::applicata z)
{
    Point point{ x,y,z };
    return point;
}
#endif /* SEMANTICALLY_INCORRECT_CODE */

#ifndef SEMANTICALLY_INCORRECT_CODE
Point get_point(axis::abscissa x, axis::ordinate y, axis::applicata z)
{
   return Point{ x,y,z };
}
#endif /* !SEMANTICALLY_INCORRECT_CODE */

int main()
{
    /*********************************************************************************************/ 
    /* Нарушение порядка вычесления по причине неучета
     * приоритета операций */

    constexpr int a{ 2 }, b{ 5 }, c{ 10 };

#ifdef SEMANTICALLY_INCORRECT_CODE
    std::cout << a + b * c << '\n'; /* подразумевалось (a + b)* c */
#endif /* SEMANTICALLY_INCORRECT_CODE */

#ifndef SEMANTICALLY_INCORRECT_CODE
    std::cout << (a + b) * c << '\n';
#endif /* !SEMANTICALLY_INCORRECT_CODE */

    /*********************************************************************************************/
    /* Некорректные значения в цикле */

    constexpr int arr[]{ -1,0,100,204 };
    constexpr size_t size{ sizeof(arr) / sizeof(arr[0]) };

#ifdef SEMANTICALLY_INCORRECT_CODE
    for (size_t i{ size - 1 }; i >= 0; --i) {
        std::cout << arr[i];
    }
#endif /* SEMANTICALLY_INCORRECT_CODE */

#ifndef SEMANTICALLY_INCORRECT_CODE
    for (size_t i{ size }; i > 0; --i) {
       std::cout << arr[i - 1] << ' ';
    }
    std::cout << '\n';
#endif /* !SEMANTICALLY_INCORRECT_CODE */

    /*********************************************************************************************/
    /* Возврат адреса локальной переменной функции */

    auto point = get_point(100.2345, 1234.0056, -12345.1235);

    /*********************************************************************************************/
    /* Применение пары "голых" операторов new / delete (new[] / delete[]):
     * 1. Отсутствие в коде ответного delete / delete[] - учетка памяти.
     * 2. Удаление объекта, на который существует еще указатель, который
     *    используется после уничтожения.
     * 3. Удаление объекта, ранее уже удаленного */

#ifdef SEMANTICALLY_INCORRECT_CODE
    {
        int* p = new int{100};
    }
    {
        int* p = new int{100};
        int* pp = p;

        delete p;
        std::cout << *pp << '\n';
    }
    {
        int* p = new int{ 100 };
        ;
        delete p;
        ;
        delete p;
    }
    
#endif /* SEMANTICALLY_INCORRECT_CODE */

#ifndef SEMANTICALLY_INCORRECT_CODE
    {
        auto p = 
            std::make_unique<Point>(get_point(100.2345, 1234.0056, -12345.1235));
    }
#endif /* !SEMANTICALLY_INCORRECT_CODE */

    return 0;
}