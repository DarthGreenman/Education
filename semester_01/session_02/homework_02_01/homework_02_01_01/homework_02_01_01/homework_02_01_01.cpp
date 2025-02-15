// homework_02_01_01.cpp

#include <fstream>
#include <iostream>
#include <exception>
#include <string>

#include "array.h"

#define DEBUG

namespace array {
    void view(const seq::Array<int>&);

    enum class direction : int {
        left = -1, right = 1
    };
    void shift(seq::Array<int>&, const direction&, size_t = 1);

    seq::Array<int> read_from_file(std::ifstream&, size_t);
    void write_to_file(std::ofstream&, const seq::Array<int>&);
}

int main()
{
    setlocale(LC_ALL, "rus");
    using std::cerr;
    using seq::Array;

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
        cerr << "Ошибка открытия файла. Проверьте наличие файла по заданному пути.\n";
        return 1;
    }

    Array<Array<int>> pair_arrays(2);
    for (size_t i{}; i < 2; ++i) {
        size_t size{}; fin >> size;
        pair_arrays[i] = array::read_from_file(fin, size);
    }
    fin.close();

#ifdef DEBUG
    array::view(pair_arrays[0]);
    array::view(pair_arrays[1]);
#endif /* DEBUG */

    try {
        array::shift(pair_arrays[0], array::direction::left);
    #ifdef DEBUG
        array::view(pair_arrays[0]);
    #endif /* DEBUG */
    }
    catch (const std::exception e) {
        cerr << e.what();
    }

    try {
        array::shift(pair_arrays[1], array::direction::right);
    #ifdef DEBUG
        array::view(pair_arrays[1]);
    #endif /* DEBUG */
    }
    catch (const std::exception e) {
        cerr << e.what();
    }

    std::ofstream fou("out.txt");
    if (fou) {
        array::write_to_file(fou, pair_arrays[1]);
        array::write_to_file(fou, pair_arrays[0]);
    }
    else {
        cerr << "Невозможно открыть выходной файловый поток для записи.\n";
        return 1;
    }
    fou.close();

    return 0;
}

namespace array {
    void view(const seq::Array<int>& array)
    {
        for (auto p = array.cbegin(); p != array.cend(); ++p) {
            std::cout << *p << '\t';
        }
        std::cout << '\n';
    }

    void shift(seq::Array<int>& array, const direction& value,
        size_t number_of_shifts)
    {
        if (number_of_shifts >= array.size()) {
            const std::string message = "Сдвиг на " +
                std::to_string(number_of_shifts) + " не имеет смысла.\n";
            throw std::exception(message.c_str());
        }
        switch (value) {
            case array::direction::left:
                for (size_t n{}; n < number_of_shifts; ++n) {
                    for (size_t i{}; i < array.size() - 1; ++i) {
                        std::swap(array[i], array[i + 1]);
                    }
                }
                break;
            case array::direction::right:
                for (size_t n{}; n < number_of_shifts; ++n) {
                    for (size_t i{ array.size() - 1 }; i > 0; --i) {
                        std::swap(array[i], array[i - 1]);
                    }
                }
                break;
        }
    }

    seq::Array<int> read_from_file(std::ifstream& fin, size_t size)
    {
        seq::Array<int> result{};
        for (int i{}, number{}; i < size && fin >> number; ++i) {
            result.push_back(number);
        }
        return result;
    }

    void write_to_file(std::ofstream& fou, const seq::Array<int>& array)
    {
        fou << array.size() << std::endl;
        for (auto p = array.cbegin(); p != array.cend(); ++p) {
            fou << *p << ' ';
        }
        fou << std::endl;
    }
}
