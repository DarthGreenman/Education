// decorator_test.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "decorator_text.h"
#include <iostream>
#include <memory>

int main()
{
    using namespace std;
 
    /*
    auto text_block = 
        make_unique<ItalicText>(make_unique<BoldText>(make_unique<Text>()));
    text_block->render("Hello world");
    std::cout << "\n";

    
    auto paragraph = make_unique<Paragraph>(make_unique<Text>());
    paragraph->render("Hello world");
    std::cout << "\n";

    auto reversed = make_unique<Reversed>(make_unique<Text>());
    reversed->render("Hello world");
    std::cout << "\n";
     */
    auto link = make_unique<Link>(make_unique<Text>());
    link->render("netology.ru", "Hello world");
   
    
    std::cout << '\n';
    std::system("pause");
    return 0;
}