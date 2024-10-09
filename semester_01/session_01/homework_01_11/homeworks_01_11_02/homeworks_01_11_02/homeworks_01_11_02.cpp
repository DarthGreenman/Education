// homeworks_01_11_02.cpp

#include <iostream>
#include <string>
#include <vector>

#include <Windows.h>

namespace bag {
    const bool find(const std::string&,const std::vector<std::string>&);
    const bool equal_case_insensitive(const std::string&, const std::string&);
    const std::string tolower(const std::string&);
}

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251); /* установка кодовой страницы win - cp1251 в поток ввода */
    SetConsoleOutputCP(1251); /* установка кодовой страницы win - cp1251 в поток вывода */

    using std::string;
    using std::vector;
    using std::cout;
    using std::cin;

    const vector<string> autumn_berries{"Калина", "Шиповник", "Облепиха",
        "Рябина", "Барбарис", "Голубика", "Брусника", "Морошка"};
    const std::pair<char, char> ans{'д', 'н'};
  
    for(char key{ ans.first }; key == ans.first;) {
        cout << "Напишите какая ягода созревает осенью?: ";
        string berry;
        cin >> berry;
        if (bag::find(berry, autumn_berries)) {
            cout << "Правильно! " << berry << " созревает осенью.\n";

            cout << "Хотите продолжить: нажмите 'д',\n";
            cout << "для окончания нажмите 'н': ";
            do {
                cin >> key;
            } while (key != ans.first && key != ans.second);
        }
        else {
            cout << "Неправильно.\n";
        }
    }
    return 0;
}

namespace bag {
    const bool find(const std::string& berry, 
        const std::vector<std::string>& list_berries)
    {
        for (const std::string& x : list_berries) {
            if (bag::equal_case_insensitive(berry, x)) {
                return true;
            }
        }
        return false;
    }

    const bool equal_case_insensitive(const std::string& lhs,
        const std::string& rhs)
    {
        return bag::tolower(lhs) == bag::tolower(rhs);
    }

    const std::string tolower(const std::string& str)
    {
        std::string tmp{ str };
        for (char& ch : tmp) {
            ch = std::tolower(ch);
        }
        return tmp;
    }
}
