// homework_01_08_01.cpp

#include <iostream>

void counting_function();

int main(int argc, char** argv)
{
    for (int i = 0; i < 15; i++) {
        counting_function();
    }
    return 0;
}

void counting_function() {
    static int counter{ 0 };
    std::cout << "Количество вызовов функции counting_function(): "
        << ++counter << '\n';
}
