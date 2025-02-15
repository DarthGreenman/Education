// homework_02_08_01.cpp
/*
* Создайте функцию int function(std::string str, int forbidden_length).
* Эта функция должна возвращать длину переданной строки, если эта длина
* не равна значению переданного параметра forbidden_length.
* В противном случае она должна выбрасывать исключение bad_length.
* 
* Для проверки функции организуйте работу с пользователем.
* В начале задайте запретную длину, а затем считывайте пользовательские
* строки и выводите их длину до тех пор, пока пользователь не введёт строку
* запретной длины. После этого сообщите ему, что он ввёл строку запретной длины,
* и завершите программу.
*/

#include <iostream>
#include <string>

#include "localisation.h"
#include "Exception.h"

size_t function(const std::string&, int);

namespace bag {
    template <typename T>
    T get_input_value(const std::string&, const std::string & =
        "Введите корректное значение для данного типа."
    );
}

int main()
{
    bag::Console_localisation local{ 
        bag::Console_localisation::Code_pages::rus 
    };
    
    using std::cin;
    using std::cout;
    for(const int len_max{
        bag::get_input_value<int>("Введите запретную длину: ") }; ; )
    {
        cout << "Введите слово: ";
        std::string word{}; cin >> word;
        try {
            const size_t len{ function(word, len_max) };
            cout << "Длина слова \"" + word + "\" равна: " << len << '\n';
        } 
        catch (const bag::Bad_length& e) {
            cout << e.what() << " До свидания!\n";
            break;
        }
    }

    std::system("pause");
    return 0;
}

size_t function(const std::string& word, int length)
{
    /* Думаю, что не совсем правильно приводить size_t к int
     * так как size_t занимает 32/64 бита, а int - 16/32
     * поэтому изменил тип возвращаемого значения. */
    return word.length() != length ? word.length() :
        throw bag::Bad_length("Вы ввели слово запретной длины!");
}

namespace bag {
    template <typename T>
    T get_input_value(const std::string& invite, const std::string& alarm)
    {
        while (true) {
            using std::cout;
            using std::cin;
            cout << invite;
            T res{};
            cin >> res;
            if (cin.fail()) {
                cin.clear();
                cin.ignore((std::numeric_limits<std::streamsize>::max)(),
                    '\n');
                cout << alarm << '\n';
            }
            else {
                cin.ignore((std::numeric_limits<std::streamsize>::max)(),
                    '\n');
                return res;
            }
        }
    }
}