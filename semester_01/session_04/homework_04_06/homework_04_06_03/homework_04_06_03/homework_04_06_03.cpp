// homework_04_06_03.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "lib/localisation.h"
#include "lib/my_vector.h"

#include <algorithm>
#include <iostream>
#include <type_traits>
#include <utility>

struct My {
    My() = default;
    My(double D, int I) : D_{ D }, I_{ I } {}

    ~My() = default;

    double  D_{};
    int     I_{};
};

int main()
{
    const bag::Console_localisation locale{
        bag::Console_localisation::Code_pages::rus
    };

    const auto print = [](int Elem) -> void { std::cout << Elem << ' '; };    
    try {
        std::cout << "Инициализация элементов массива списком:\nconst seq::My_vector<int> vec_1{ ... };\n";
        const seq::My_vector<int> vec_1{ 0,1,2,3,4,5,6,7,8,9,10 };
        std::for_each(std::cbegin(vec_1), std::cend(vec_1), print);
        std::cout << '\n' << "size: " << vec_1.size() << '\t' << "capacity: " << vec_1.capacity();
        std::cout << "\n\n";
      
        std::cout << "Инициализация элементов массива копированием:\nseq::My_vector<int> vec_2{ vec_1 };\n";
        seq::My_vector<int> vec_2{ vec_1 };
        std::for_each(std::cbegin(vec_2), std::cend(vec_2), print);
        std::cout << '\n' << "size: " << vec_2.size() << '\t' << "capacity: " << vec_2.capacity();
        std::cout << "\n\n";
        
        std::cout << "Добавление элементов в конец массива:\nvec_2.push_back(...);\n";
        constexpr size_t num{ 10 };
        for (size_t i{}; i < num; ++i) 
        {
            vec_2.push_back(static_cast<int>(num + 1 + i));
        }
        std::for_each(std::cbegin(vec_2), std::cend(vec_2), print);
        std::cout << '\n' << "size: " << vec_2.size() << '\t' << "capacity: " << vec_2.capacity();
        std::cout << "\n\n";
        
        std::cout << "Инициализация элементов массива перемещением:\nseq::My_vector<int> vec_3{ std::move(vec_2) };\n";
        seq::My_vector<int> vec_3{ std::move(vec_2) };
        std::for_each(std::cbegin(vec_3), std::cend(vec_3), print);
        std::cout << '\n' << "size: " << vec_3.size() << '\t' << "capacity: " << vec_3.capacity();
        std::cout << "\n\n";

        std::cout << "Инициализация элементов массива размером 4 нулями:\nseq::My_vector<int> vec_2{ vec_1 };\n";
        seq::My_vector<int> vec_4(4);
        std::for_each(std::cbegin(vec_4), std::cend(vec_4), print);
        std::cout << '\n' << "size: " << vec_4.size() << '\t' << "capacity: " << vec_4.capacity();
        std::cout << "\n\n";
        std::cout << "Операция присваивания копированием:\nvec_4 = vec_1;\n";
        vec_4 = vec_1;
        std::for_each(std::cbegin(vec_4), std::cend(vec_4), print);
        std::cout << '\n' << "size: " << vec_4.size() << '\t' << "capacity: " << vec_4.capacity();
        std::cout << "\n\n";
       
        std::cout << "Инициализация пустого массива элементами типа My:\nseq::My_vector<My> vec_m\n";
        seq::My_vector<My> vec_m;
        std::cout << "Добавление элементов в конец массива:\nvec_m.push_back(...);\n";
        for (size_t i{}; i < num + 2; ++i)
        {
            My elem{ static_cast<double>(i * i), static_cast<int>(i) };
            vec_m.push_back(elem);
        }

        std::for_each(std::cbegin(vec_m), std::cend(vec_m), 
            [](const My Elem) -> void {
            static std::size_t count{ 0 };
            auto&& [d, i] = Elem;
            std::cout << ++count << '\t' << d << '\t' << i << '\n'; }
        );
        std::cout << '\n' << "size: " << vec_m.size() << '\t' << "capacity: " << vec_m.capacity();
        std::cout << "\n\n";
    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }

    std::system("pause");
    return 0;
}