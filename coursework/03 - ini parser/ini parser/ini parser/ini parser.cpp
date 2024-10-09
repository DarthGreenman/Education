// ini parser.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

// Формат ini:
// 1. Пробелы и табуляции вначале строки игнорируются.
// 2. Комментарии могу начинаиться в начале строки, или быть продолжением.
// 3. Заголовки должны начинаться с '[' и заканнчиваться ']'. Имя заголовка может состоять из букв, цифр, '_',
//    но перый символ должен быть буквой.
// 4. Строка КЛЮЧ - ЗНАЧЕНИЕ должна иметь символ '=', но идущие подряд '=' вызывают исключение. 
// 5. Имя ключа может состоять из букв, цифр, '_', но перый символ должен быть буквой.
// * перечень разрешенных символов см. в my_parser.cpp, строка 40.

#include "../../include/my_parser.h"

#include <cstdlib>
#include <exception>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <string>
#include <utility>

int main()
{   
    std::ifstream file{ "..\\..\\test_1.ini" };
    if (!file.is_open())
        return 1;

    try {
        const my::parser ini{ file };
        auto num = ini.get_value<int>("drivers.timer");
        std::cout << num << '\n';
        std::cout << ++num << '\n';
        std::cout << ini.get_value<std::string>(std::make_pair("386Enh", "CGA80WOA.FON"));
        std::cout << ini.get_value<std::string>(std::make_pair("386Enh", "CG80WOA.FON"));
    }
    catch (const std::exception& err) {
        file.close();
        std::cout << err.what();
    }

    std::system("pause");
    return 0;
}