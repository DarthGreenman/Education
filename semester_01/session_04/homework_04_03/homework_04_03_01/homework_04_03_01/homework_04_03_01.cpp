// homework_04_03_01.cpp
/*
 * Вам нужно реализовать умный массив для данных типа int в духе идиомы RAII,
 * который сам управляет своей памятью: самостоятельно выделяет её при создании и очищает,
 * когда объект данного класса не используется.
 * Должны быть реализованы следующие функции:
 *  1. Конструктор, принмающий количество элементов, которое будет хранить массив.
 *  2. Функция добавления нового элемента в массив. Не забудьте обработать случай,
 *     когда количество элементов больше количества элементов, на которую выделена память.
 *  3. Функция получения элемента по индексу. Не забудьте проверку на корректность индекса.
 *  4. Деструктор.
*/

#include "array.h"
#include <iostream>
#include <memory>
#include <stdexcept>

namespace geo
{
    template<typename T>
    struct Point
    {
        using name      = char;
        using abscissa  = T;
        using ordinate  = T;
        using applicata = T;
    
        Point() = default;
        Point(char N, abscissa X, ordinate Y, applicata Z)
            : n_{ N }, x_ { X }, y_{ Y }, z_{ Z } {}

        name        n_{};
        abscissa    x_{};
        ordinate    y_{};
        applicata   z_{};
    };

}
int main()
{
	std::unique_ptr<seq::Array<geo::Point<int>>> surface{};
    try {
        surface = std::make_unique<seq::Array<geo::Point<int>>>();
        surface.get()->push_back(geo::Point{ 'Y',1,0,0 });
        surface.get()->push_back(geo::Point{ 'Z',0,1,0 });
        surface.get()->push_back(geo::Point{ 'A',0,0,1 });
        surface.get()->push_back(geo::Point{ 'R',5,3,1 });
        surface.get()->push_back(geo::Point{ 'B',0,0,0 });
        surface.get()->push_back(geo::Point{ 'H',1,0,9 });
    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }

    std::sort(surface.get()->begin(), surface.get()->end(),
        [](const geo::Point<int> lhs, const geo::Point<int> rhs) { return lhs.n_ < rhs.n_; });
   
    std::for_each(surface.get()->cbegin(), surface.get()->cend(), 
        [](const geo::Point<int> point)
    {
        auto&& [name,x, y, z] = point;
        std::cout << name << ": " << x << ", " << y << ", " << z << '\n';
    });
        
	return 0;
}