// partial_function_specialisation.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>


struct dog {
    int age{};
    double weight{};
};

template<typename T, typename U>
void function(T a, U b) {
    std::cout << a << '\t' << b << '\n';
}

template<typename U>
void function(dog a, U b) {
    std::cout << a.age << '\t' << b << '\n';
}

void function(dog a, int b) {
    std::cout << a.age << '\t' << b << '\n';
}

int main()
{
    function(2, 3);
    function(dog{ 10,30.5 }, 12);

    return 0;
}