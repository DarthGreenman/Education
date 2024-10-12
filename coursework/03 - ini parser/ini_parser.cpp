// ini parser.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

// Формат ini:
// 1. Пробелы и табуляции вначале строки игнорируются.
// 2. Комментарии могу начинаиться в начале строки, или быть продолжением.
// 3. Заголовки должны начинаться с '[' и заканнчиваться ']'. Имя заголовка может состоять из букв, цифр, '_',
//    но перый символ должен быть буквой.
// 4. Строка КЛЮЧ - ЗНАЧЕНИЕ должна иметь символ '=', но идущие подряд '=' вызывают исключение. 
// 5. Имя ключа может состоять из букв, цифр, '_', но перый символ должен быть буквой.
// * перечень разрешенных символов см. в my_parser.cpp, строка 40.

#include "my_parser.h"

#include <exception>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <string>

int main()
{
    std::ifstream file{ "ini\\test_2.ini" };
    if (!file.is_open())
        return 1;

    try {
        const ini::parser ini{ file };
        const auto num_1 = ini.get_value<float>({ { "Section1" }, { "var1" } });
        const auto num_2 = ini.get_value<unsigned>("Section2.var1");
        std::cout << num_1 + num_2 << '\n';
        std::cout << ini.get_value<std::string>({ { "Section1" }, { "var2" } });
    }
    catch (const std::exception& err) {
        file.close();
        std::cout << err.what();
    }

    return 0;
}