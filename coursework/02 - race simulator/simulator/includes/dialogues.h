/* Dialogues.h */

#ifndef RACE_DIALOGUES_H
#define RACE_DIALOGUES_H

#include "types.h"
#include "menubar.h"
#include "racer.h"

#include <vector>
#include <string>
#include <memory>
#include <iomanip>
#include <iostream>

namespace ui
{
    using Menu = std::vector<std::pair<std::string, Barkey>>;
   
    /* Построители меню */
    Menu make_menu(const std::string& menu_raw, char del = '|',
        const std::string& quit = "ВЫЙТИ ИЗ СИМУЛЯТОРА");
    Menu make_menu(char bar_key, const std::string& quit);
    std::vector<std::pair<vehicles::Typeid, std::string>> make_list_racers();

    bool key_exists(const Menu& menu, char bar_key);    
    std::string get_name_racer(const Menu& menu, char bar_key);
    
    /* Вывод информации */
    void view_info_racing(const std::vector<std::unique_ptr<vehicles::Vehicle>>& race);

    template<typename N, typename V>
    concept Types_row = std::same_as<N, char>&& std::same_as<V, std::string> ||
        std::same_as<N, size_t>&& std::same_as<V, char> ||
        std::same_as<N, size_t>&& std::same_as<V, double>;

    template<typename N, typename V>
        requires Types_row<N, V>
    struct Row {
        std::pair<size_t, N>            pos;
        std::pair<size_t, std::string>  name;
        std::pair<size_t, V>            val;
    };

    template<typename N, typename V>
    void view(const Row<N, V>& row)
    {
        auto&& [pos, name, val] = row;
        std::cout << std::setw(pos.first) << std::left << pos.second;
        std::cout << std::setw(name.first) << std::left << name.second;
        std::cout << std::setw(val.first) << std::right << val.second;
        std::cout << '\n';
    }

    template<typename T>
    concept Type_value = std::same_as<T, Menu> || 
        std::same_as<T, std::vector<std::pair<std::string, double>>>;

    template<typename N, typename V, typename T>
        requires Type_value<T>
    void view(const Row<N, V>& header, const T& value)
    {
        ui::view(header);
        size_t num{};
        for (const auto& row : value) {
            auto&& [name, res] = row;
            if (std::is_same<T, Menu>::value) {
                view<size_t, char>(
                    { {5, ++num},{30, name}, {10, static_cast<char>(res)} }
                );
            }
            else {
                view<size_t, double>(
                    { {5, ++num},{30, name}, {10, static_cast<double>(res)} }
                );
            }
        }
    }
};
#endif /* RACE_DIALOGUES_H */

