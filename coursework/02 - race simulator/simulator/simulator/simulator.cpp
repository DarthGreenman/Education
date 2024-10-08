/* simulator.cpp */

#include "types.h"
#include "race.h"

#include "localisation.h"
#include "dialogues.h"
#include "input.h"

#include <iostream>
#include <string>
#include <cctype>
#include <utility>
#include <algorithm>
#include <vector>

int main()
{
    using ui::make_menu;
    using ui::Menu;
    using ui::view;
    using bag::get_input_value;
    using std::cout;
    using std::cin;

    bag::Console_localisation locale{
        bag::Console_localisation::Code_pages::rus
    };

    /* ВЫБОР ТИПА ГОНКИ */
    for (char key{'Y'}; std::toupper(key) != static_cast<char>(ui::Barkey::QUIT); )
    {
        std::system("cls");
        cout << "ДОБРО ПОЖАЛОВАТЬ В ГОНОЧНЫЙ СИМУЛЯТОР!\n\n";
        auto menu_main = make_menu("Наземный|Воздушный|"
            "Наземный и воздушный");
        view<char, std::string, Menu>({ {5, 'N'}, {30, "ТРАНСПОРТ"}, {10, "ВЫБОР"} },
            menu_main); /* Меню выбора типа гонки */

        char bar_key{};
        for (; get_input_value(cin, bar_key, "Выберите : ") &&
            !ui::key_exists(menu_main, bar_key); ) {
        }

        if (std::toupper(bar_key) == static_cast<char>(ui::Barkey::QUIT)) {
            cout << "\nЗАКОНЧИТЬ (Q), ИЛИ НАЧАТЬ НОВУЮ СИМУЛЯЦИЮ (Y)?: "; 
            cin >> key;
            continue; /* Выход */
        }

        /* РЕГИСТРАЦИЯ УЧАСТНИКОВ ГОНКИ, ФОРМИРОВАНИЕ КОМАНДЫ ГОНЩИКОВ */
        cout << "\n\nРЕГИСТРАЦИЯ\n\n";
        const auto menu_racers = make_menu(bar_key, "ЗАКОНЧИТЬ РЕГИСТРАЦИЮ");
        view<char, std::string, Menu>({ {5, 'N'}, {30, "УЧАСТНИК"}, {10, "ВЫБОР"} },
            menu_racers); /* Меню выбора участников */

        std::vector<std::unique_ptr<vehicles::Vehicle>> team{};
        for (std::string message{ "Выберите: " };
            std::toupper(bar_key) != static_cast<char>(ui::Barkey::QUIT);)
        {
            for (; get_input_value(cin, bar_key, message) &&
                std::toupper(bar_key) != static_cast<char>(ui::Barkey::QUIT); )
            {
                if (ui::key_exists(menu_racers, bar_key)) /* Проверка наличия bar_key в меню */
                {      
                    const std::string name = ui::get_name_racer(menu_racers, bar_key);
                    if ( const auto it = std::find_if(std::begin(team), std::end(team),
                        [&name](std::unique_ptr<vehicles::Vehicle>& racer) { return racer.get()->name() == name; });
                        it == std::end(team)) /* Проверка регистрации участника */
                    { /* Не зарегистрированный */
                        team.push_back(std::move(race::make_racer(bar_key)));
                        cout << "\nЗАГЕРИСТРИРОВАННЫЕ УЧАСТНИКИ:\n";
                        ui::view_info_racing(team);
                        message = "\nПродолжить выбор: ";
                    }
                    else {
                        message = "Участник был зарегистрирован ранее.\n"
                            "Выберите другого участника: ";
                    }
                }
                else { /* bar_key отсутствует в меню */
                    message = "Повторите ввод: ";
                }
            }
            if (team.size() < 2) {
                message = "Должны быть зарегистрированы как минимум 2 участника.\n"
                    "Выберите: ";
                bar_key = 'N';
            }
        }
        std::system("cls");

        cout << "РЕГИСТРАЦИЯ УЧАСТНИКОВ ГОНКИ ЗАКОНЧЕНА";
        /* КОМАНДА ГОНЩИКОВ СФОРМИРОВАНА, ВВОД ДИСТАНЦИИ И СТАРТ */
        race::Race lap{ std::move(team),
            get_input_value<double>("\nВведите длину дистанции: ") };

        cout << "\n\nСТАРТ\n\n";
        menu_main = make_menu("Начать гонку");
        view<char, std::string, Menu>({ {5, 'N'}, {30, "ДЕЙСТВИЕ"}, {10, "ВЫБОР"} },
            menu_main); /* Меню выбора типа гонки */

        for (; get_input_value(cin, bar_key, "Выберите : ") &&
            !ui::key_exists(menu_main, bar_key); ) {
        }

        if (std::toupper(bar_key) == static_cast<char>(ui::Barkey::QUIT)) {
            cout << "\nЗАКОНЧИТЬ (Q), ИЛИ НАЧАТЬ НОВУЮ СИМУЛЯЦИЮ (Y)?: ";
            cin >> key;
            continue; /* Выход */
        }

        lap.start();
        auto result = lap.finish();
        /* Сортировка участников по результату */
        std::sort(std::begin(result), std::end(result), [](const std::pair<std::string, double>& lhs,
            const std::pair<std::string, double>& rhs) { return lhs.second < rhs.second; }
        );
        cout << "\n\nТАБЛИЦА РЕЗУЛЬТАТОВ\n\n";
        view<char, std::string, std::vector<std::pair<std::string, double>>>
            ({ {5, 'N'}, {30, "УЧАСТНИК"}, {10, "ВРЕМЯ"} }, result);

        cout << "\nЗАКОНЧИТЬ (Q), ИЛИ НАЧАТЬ НОВУЮ СИМУЛЯЦИЮ (Y)?: ";
        cin >> key;
    }
    return 0;
}