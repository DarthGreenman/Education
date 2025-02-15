// homework_04_07_02.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
// Нужно реализовать шаблонную функцию, которая печатает содержимое контейнера.
// Контейнер может быть любым: std::set, std::vector, std::list
// Пример правильной работы программы:
// std::set<std::string> test_set = { "one", "two", "three", "four" };
// print_container(...); // four one three two. помните почему такой порядок? :)
// 
// std::list<std::string> test_list = { "one", "two", "three", "four" };
// print_container(...); // one, two, three, four
// 
// std::vector<std::string> test_vector = { "one", "two", "three", "four" };
// print_container(...); // one, two, three, four


#include <iostream>
#include <list>
#include <set>
#include <string>
#include <vector>

namespace seq
{    
    template<typename T>
    struct Print_data {
        void operator()(const T& Value)
        {
            std::cout << Value << ' ';
        }
    };

    template<>
    struct Print_data<std::string> {
        void operator()(const std::string& Value)
        {
            std::cout << '"' << Value << '"' << ' ';
        }
    };

    template<typename InputIt, typename Print = Print_data<std::string>>
    void print_container(InputIt First, InputIt Last, Print Func = Print{});

    struct Person
    {
        std::string name{};
        unsigned age{};
    };
}
int main()
{
    // Печать контейнеров с типом элементов std::string
    const std::set<std::string> set_s{ "one", "two", "three", "four" };
    std::cout << "Print std::set<std::string> set_s{ ... }\n";
    seq::print_container(std::cbegin(set_s), std::cend(set_s));
    
    const std::list<std::string> list_s{ "one", "two", "three", "four" };
    std::cout << "\n\nPrint const std::list<std::string> list_s{ ... }\n";
    seq::print_container(std::cbegin(list_s), std::cend(list_s));

    const std::vector<std::string> vector_s{ "one", "two", "three", "four" };
    std::cout << "\n\nPrint const std::vector<std::string> vector_s{ ... }\n";
    seq::print_container(std::cbegin(vector_s), std::cend(vector_s));

    // Печать контейнеров с типом элементов int
    const std::set<int> set_i{ 0,1,2,3,4,5,6,7,8,9 };
    std::cout << "\n\nPrint std::set<int> set_i{ ... }\n";
    seq::print_container(std::cbegin(set_i), std::cend(set_i), 
        seq::Print_data<int>{});

    const std::list<int> list_i{ 0,1,2,3,4,5,6,7,8,9 };
    std::cout << "\n\nPrint const std::list<int> list_i{ ... }\n";
    seq::print_container(std::cbegin(list_i), std::cend(list_i),
        seq::Print_data<int>{});

    const std::vector<int> vector_i{ 0,1,2,3,4,5,6,7,8,9 };
    std::cout << "\n\nPrint const std::vector<int> vector_i{ ... }\n";
    seq::print_container(std::cbegin(vector_i), std::cend(vector_i),
        seq::Print_data<int>{});

    // Печать контейнера std::vector с типом элементов Person
    const auto print_person = [](const seq::Person& Visitor) ->void {
        auto&& [name, age] = Visitor;
        std::cout << "Name:\t" << name << '\n';
        std::cout << "Age:\t" << age << "\n\n";
    };
    
    const std::vector<seq::Person> vector_p{
        seq::Person{"Ivan",     16},
        seq::Person{"Pavel",    26},
        seq::Person{"Vladimir", 30} 
    };
    std::cout << "\n\nPrint const std::vector<seq::Person> vector_p{ ... }\n";
    seq::print_container(std::cbegin(vector_p), std::cend(vector_p), print_person);

    std::cout << '\n';
    std::system("pause");
    return 0;
}

namespace seq 
{
    template<typename InputIt, typename Print>
    void print_container(InputIt First, InputIt Last, Print Func)
    {
        for (; First != Last; ++First) {
            Func(*First);
        }
    }
}
