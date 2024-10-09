// homework_01_12_01.cpp 

#include <iostream>
#include <fstream>
#include <string>
#include <locale>

#include <Windows.h>

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    using std::cout;

    std::ifstream fin("in.txt");
    if (!fin.is_open()) {
        cout << "Файл не может быть открыт!\n";
        return 1;
    }

    std::string word;
    while (fin >> word) {
        cout << word << '\n';
    }

    fin.close();

	return 0;
}