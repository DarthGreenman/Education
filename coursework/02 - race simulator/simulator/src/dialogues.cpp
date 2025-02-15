/* Dialogues.cpp */

#include "dialogues.h"
#include "vehicle_land.h"
#include "vehicle_air.h"
#include "types.h"

#include <sstream>

namespace ui
{
    Menu make_menu(const std::string& str_raw,
        char del, const std::string& quit)
    {
        std::stringstream stream{ str_raw };
        std::string bar_name;
        std::vector<std::pair<std::string, Barkey>> menu{};

        auto bar_key{ Barkey::BAR_01 };

        while (std::getline(stream, bar_name, del))
        {
            const std::pair<std::string, Barkey> bar{
                std::make_pair(bar_name, bar_key++)
            };
            menu.push_back(bar);
        }
        menu.push_back(std::make_pair(quit, Barkey::QUIT));
        return menu;
    }

    Menu make_menu(char bar_key, const std::string& quit)
    {
        static auto race = ui::make_list_racers();
        std::vector<std::pair<std::string, Barkey>> menu{};

        for (const auto& racer : race)
        {
            auto&& [key, name] = racer;
            if (std::toupper(bar_key) == static_cast<char>(Barkey::BAR_01) ||
                std::toupper(bar_key) == static_cast<char>(Barkey::BAR_02))
            {
                if (std::toupper(bar_key) == static_cast<char>(key.habitat)) {
                    menu.push_back(std::make_pair(name, key.bar_key));
                }
            }
            else {
                menu.push_back(std::make_pair(name, key.bar_key));
            }
        }
        menu.push_back(std::make_pair(quit, Barkey::QUIT));
        return menu;
    }

    bool key_exists(const Menu& menu, char menu_key)
    {
        for (const auto& bar : menu) {
            if (std::toupper(menu_key) == static_cast<char>(bar.second)) {
                return true;
            }
        }
        return false;
    }

    std::vector<std::pair<vehicles::Typeid, std::string>> make_list_racers()
    {
        using namespace vehicles::types;
        const std::vector<std::pair<vehicles::Typeid, std::string>> menu{
            {std::make_pair(Bactrian<size_t>::key,       Bactrian<size_t>::name)},
            {std::make_pair(Dromedar<size_t>::key,  Dromedar<size_t>::name)},
            {std::make_pair(Centaur<size_t>::key,     Centaur<size_t>::name)},
            {std::make_pair(Boots<size_t>::key,       Boots<size_t>::name)},
            {std::make_pair(Carpet<double>::key,      Carpet<double>::name)},
            {std::make_pair(Eagle<double>::key,       Eagle<double>::name)},
            {std::make_pair(Broom<double>::key,       Broom<double>::name)}
        };
        return menu;
    }

    std::string get_name_racer(const Menu& menu, char bar_key)
    {
        for (const auto& menu_bar : menu) {
            if (static_cast<char>(menu_bar.second) == std::toupper(bar_key)) {
                return menu_bar.first;
            }
        }
        return std::string{};
    }

    void view_info_racing(const std::vector<std::unique_ptr<vehicles::Vehicle>>& race)
    {
        size_t i{};
        for (const auto& racer : race) {
            std::cout << ++i << ' ' << racer.get()->name() << '\n';
        }
    }
}