// chain_of_responsibility_test.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "include/handler.h"
#include "include/handlers.h"
#include "include/logger.h"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <string>

std::string get_error() { return "Error message"; }
std::string get_fatal() { return "Fatal error message"; }

int main()
{
    using logger = pattern::chain_of_responsibility::logger;
    auto& log = logger::get_instance();
    
    auto lev_w = std::make_shared<pattern::chain_of_responsibility::warning>();
    auto lev_e = std::make_shared<pattern::chain_of_responsibility::error>("log.txt");
    auto lev_f = std::make_shared<pattern::chain_of_responsibility::fatal>();

    lev_w->next(lev_e);
    lev_e->next(lev_f);

    /// Формируем сообщение об ошибке по какому-то событию 
    lev_w->receiver(log.send(logger::level::warning, "Warning"));
    
    /// Получаем сообщение об ошибке из какого-то источника
    lev_w->receiver(log.send(logger::level::error, get_error()));

    try {   
        auto err = get_fatal(); /// Получаем сообщение об ошибке из какого-то источника и модифицируем
        err += " - memory allocation error";
        lev_w->receiver(log.send(logger::level::fatal, err));
    }
    catch (const std::exception& err) { std::cout << err.what(); }

    try {
        lev_w->receiver(log.send(logger::level::unknown, "Unknown"));
    }
    catch (const std::exception& err) { std::cout << err.what(); }

    std::cout << '\n';
    std::system("pause");
    return 0;
}