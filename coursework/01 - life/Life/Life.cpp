// Life.cpp
/****************************************************************************************
* Для создания объектов клетка (тип space::Nucleus) в объекте Вселенная
* (тип space::Universe), программа считывает начальные данные из файла: 
* 1. Размеры Вселенной (тип space::_place синоним типа std::pair<size_t, size_t>).
* 2. Координаты ячеек - массив координат "живых" клеток.
* Если файл данных отсутствует, то программа создает файл, записывает данные и повторно
* считывает данные.
* Для формирования массива координат "живых" клеток используется генератор, который
* создает равномерное распределение целочисленных значений в диапазоне Вселенной.
* ***************************************************************************************
* По исходным данным создается контейнер клеток, со всеми инициализированными данными
* объектов типа space::Nucleus:
* 1. Статусом "живой" или "мертвый" (тип space::Nucleus::State).
* 2. Координатами положения во Вселенной (тип space::_place).
* 3. Массивом координат (тип space::_place) соседних клеток.
* ***************************************************************************************
* При отладке есть возможность визуально оценить данные созданного контейнера.
* ***************************************************************************************
* В цикле "поколение", метод space::Universe::evolution() возвращает количество изменений
* статуса клеток, количество "живых" клеток и номер поколения.
* Игра прекращается, если:
* 1. На поле не останется ни одной «живой» клетки.
* 2. При очередном шаге ни одна из клеток не меняет своего состояния.
*/

#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <string>
#include <vector>

//#include "array.h"
#include "nucleus.h"
#include "universe.h"
#include "space.h"
#include <iosfwd>
#include <clocale>

#define DEBUG

int main()
{
    setlocale(LC_ALL, "rus");
    using std::cout;
    using std::cerr;
    using std::endl;
    using std::system;

    space::_place edge{};
    //seq::Array <space::_place> alive{};
    std::vector<space::_place> alive{};
    /*
    * Тестовые данные:
    *  1. test01.dat - "Палка".
    *  2. test02.dat - "Квадрат".
    *  3. test03.dat - пример из задания.
    *  4. test04.dat - "Планер".
    *  5. test05.dat - "Квадрат".
    *  6. test06.dat - сгенерированное задание. 
    */
    const std::string path{"test06.dat"};
    std::ifstream fin{path};
    
    if (!fin.is_open()){
        fin.clear();
        std::ofstream fou{path};
        if (fou) {
            const space::_place Edge{45, 55};
            fou << Edge.first << static_cast<char>(0x20) << Edge.second << endl;
            
            const std::vector<space::_place> Alive{space::make_life(Edge, 3)};
            for (auto p = Alive.cbegin(); p != Alive.cend(); ++p) {
                auto&& [depth, width] = *p;
                fou << depth << static_cast<char>(0x20) << width << endl;
            }
        }
        else {
            cerr << "Невозможно открыть выходной файловый поток для записи.\n";
            return 1;
        }
        fou.close();
        fin.open(path);
    }
    fin >> edge.first >> edge.second;
    for (size_t depth{}, width{}; fin >> depth >> width;) {
        alive.push_back({ depth, width });
    }
    fin.close();
  
    space::Universe world(edge, alive);

#ifdef DEBUG
    view(world, space::Universe::View::points,
        "Создание пространства.\n");
    std::system("pause");
    std::system("cls");
    
    view(world, space::Universe::View::neighbors,
       "Соседи.\nОбозн.: '-' - 3 объекта; '+' - 5 об.; 'x' - 8 об.;"
        " '?' - ошибка.\n");
    std::system("pause");
    std::system("cls");
#endif /* DEBUG */
    
    for (size_t prev_number_of_living{},
        prev_changes{};;) {
        
        auto&&[number_of_living, changes, gen] = world.evolution();

        if (number_of_living == prev_number_of_living &&
            changes == prev_changes ||
            !number_of_living) {
            system("pause");
            break;
        } else {
            std::system("cls");
            view(world, space::Universe::View::evolution, "Развитие.");
            cout << "Поколение: " << gen
                << "\nКоличество мутаций: " << changes
                << "\nКоличество живых клеток: " << number_of_living << '\n';
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
              
        prev_number_of_living = number_of_living;
        prev_changes = changes;
    }
    return 0;
}
