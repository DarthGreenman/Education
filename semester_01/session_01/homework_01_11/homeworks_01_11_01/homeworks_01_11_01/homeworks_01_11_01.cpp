// homeworks_01_11_01.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <memory>

#include <cstring>
#include <cstdio>

#include "..\..\..\..\library\localisation.h";

namespace bag {
    void getline(char*, int);
}

int main()
{
    bag::localisation();
    using std::cout;
    using std::cin;

    {
        const int kMax_size_name{ 64 };

        char name[kMax_size_name];
        cout << "Введите имя: ";
        /* Можно вводить строку состоящую из двух, и более слов. */
        bag::getline(name, kMax_size_name - 1);
        
        char surname[kMax_size_name];
        cout << "Введите фамилию: ";
        /* Можно вводить строку состоящую из двух, и более слов. */
        bag::getline(surname, kMax_size_name - 1);

        const char greetings[]{ "Здравствуйте, " };
        auto answer{ std::make_unique<char[]>(strlen(greetings) +
            strlen(name) +
            strlen(surname) + 
            3) }; /* 3: strlen(" ") + strlen("!") + strlen("\0") */

        strcpy(&answer[0], greetings);
        strcat(&answer[0], name);
        strcat(&answer[0], " ");
        strcat(&answer[0], surname);
        strcat(&answer[0], "!");
        cout << answer << '\n';
    }

    {
        using std::string;
        using std::getline;
        
        string name;
        cout << "Введите имя: "; getline(cin, name);
        string surname;
        cout << "Введите фамилию: "; getline(cin, surname);
        cout << "Здравствуйте, " + name + " " + surname + "!";
    }
    return 0;
}

namespace bag {
    void getline(char* str, int len) {
        std::fgets(str, len, stdin);
        const size_t end{ strlen(str) };
        if (str[end - 1] == '\n') {
            str[end - 1] = '\0';
        }
    }
}