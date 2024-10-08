// homework_02_04_01.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "localisation.h"
#include "address.h"

struct data_block {
    std::string city;
    std::string street;
    unsigned house{};
    unsigned apartment{};
};

int main()
{
    bag::console_localisation localisation{ 1251 };
    using std::cerr;
    using std::string;

    std::ifstream fin("in.txt");
    if (!fin.is_open()) {
        /*
        * Ошибки могут включать:
        * 1. Файл не существует.
        * 2. Файл не имеет прав доступа.
        * 3. Файл уже используется.
        * 4. Произошли ошибки диска.
        * 5. Отключен диск ...
        */
        cerr << "Ошибка открытия файла. "
            "Проверьте наличие файла по заданному пути.\n";
        return 1;
    }

    std::vector<location::address> address_book;
    for (data_block data;
        fin >> data.city >> data.street >> data.house >> data.apartment; ) {
        const location::address address{
            data.city, 
            data.street, 
            data.house,
            data.apartment
        };
        address_book.push_back(address);
    }
    fin.close();

    std::ofstream fou("out.txt");
    if (fou) {
        fou << address_book.size() << '\n';
        while (!address_book.empty()){
            const location::address address{ address_book.back() };
            address_book.pop_back();
            fou << address.get_address();
            fou << '\n';
        }
    }
    else {
        cerr << "Невозможно открыть выходной файловый поток для записи.\n";
        return 1;
    }
    fou.close();

    return 0;
}