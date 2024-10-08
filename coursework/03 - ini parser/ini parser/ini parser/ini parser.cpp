// ini parser.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#include "../../include/my_parser.h"

#include <exception>
#include <fstream>
#include <iosfwd>
#include <iostream>

int main()
{   
    std::ifstream file{ "..\\..\\test_1.ini" };
    if (!file.is_open())
        return 1;

    try {
        const my::parser ini{ file };
        auto num = ini.get_value<int>("drivers:timer");
        std::cout << num << '\n';
        std::cout << ++num << '\n';
        std::cout << ini.get_value<std::string>(std::make_pair("386Enh", "CGA80WOA.FON"));
        std::cout << ini.get_value<std::string>(std::make_pair("386Enh", "CG80WOA.FON"));
    }
    catch (const std::exception& err) {
        file.close();
        std::cout << err.what();
    }

    return 0;
}