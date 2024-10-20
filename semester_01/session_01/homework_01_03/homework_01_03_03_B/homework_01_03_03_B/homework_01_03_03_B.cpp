﻿// homework_01_03_03_C.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <vector>

#include <Windows.h>

#include "Person.h"
#include "Zodiac.h"

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const vector<Zodiac> zodiac_signs {
        { { Zodiac::Sign_key::Aries, "Овен" }, { Zodiac::Nature_key::Fire,  "Огонь" } },
        { { Zodiac::Sign_key::Taurus, "Телец" },          { Zodiac::Nature_key::Earth, "Земля" } },
        { { Zodiac::Sign_key::Gemini, "Близнецы" },       { Zodiac::Nature_key::Air,   "Воздух" } },
        { { Zodiac::Sign_key::Cancer, "Рак" },            { Zodiac::Nature_key::Water, "Вода" } },
        { { Zodiac::Sign_key::Lion, "Лев" },              { Zodiac::Nature_key::Fire,  "Огонь" } },
        { { Zodiac::Sign_key::Virgo, "Дева" },            { Zodiac::Nature_key::Earth, "Земля" } },
        { { Zodiac::Sign_key::Libra, "Весы" },            { Zodiac::Nature_key::Air,   "Воздух" } },
        { { Zodiac::Sign_key::Scorpion, "Скорпион" },     { Zodiac::Nature_key::Water, "Вода" } },
        { { Zodiac::Sign_key::Sagittarius, "Стрелец" },   { Zodiac::Nature_key::Fire,  "Огонь" } },
        { { Zodiac::Sign_key::Capricorn, "Козерог" },     { Zodiac::Nature_key::Earth, "Земля" } },
        { { Zodiac::Sign_key::Aquarius, "Водолей" },      { Zodiac::Nature_key::Air,   "Воздух" } },
        { { Zodiac::Sign_key::Fish, "Рыбы" },             { Zodiac::Nature_key::Water, "Вода" } }
    };

    string name_visitor{};
    cout << "Введите имя: "; cin >> name_visitor;

    const map<Person::Gender_key, string> gender_map{
        { Person::Gender_key::Female, "Женский" },
        { Person::Gender_key::Male, "Мужской" }
    };
    
    pair<Person::Gender_key, string> gender;
    cout << "Введите пол <Мужской или Женский>: ";
    do {
        string value{};
        cin >> value;
        for (const auto& x : gender_map) {
            if (value == x.second) {
                gender = make_pair(x.first, x.second);
                break;
            }
        }
    } while (!(gender.first == Person::Gender_key::Female ||
        gender.first == Person::Gender_key::Male));
    
    unsigned age{ static_cast<unsigned>(Person::Age::Teenager) };
    cout << "Введите возраст: "; cin >> age;
    if (age > static_cast<unsigned>(Person::Age::Longlived)) {
        cout << "Поздравляю, Вы долгожитель!\n";
    }

    Zodiac zodiac;
    cout << "Введите знак зодиака <Названия с заглавной буквы>: ";
    do {
        string sign{};
        cin >> sign;
        for (const auto& x : zodiac_signs) {
            if (sign == x.get_sign().second) {
                zodiac.set_sign(x.get_sign());
                zodiac.set_nature(x.get_nature());
                break;
            }
        }
    } while (!zodiac.is_defined());
    
    Person visitor{ name_visitor, gender, age, zodiac };  
    if (visitor.get_gender().first == Person::Gender_key::Male &&
        visitor.get_zodiac().get_nature().first == Zodiac::Nature_key::Water &&
        visitor.get_age() > static_cast<unsigned>(Person::Age::Mature)) {
        cout << "\nСегодня очень плодотворный день. Можно добиться того, "
            "что прежде казалось почти невозможным.\n";
    }
    else if (visitor.get_gender().first == Person::Gender_key::Female &&
        visitor.get_zodiac().get_nature().first == Zodiac::Nature_key::Earth &&
        visitor.get_age() >= static_cast<unsigned>(Person::Age::Teenager) &&
        visitor.get_age() <= static_cast<unsigned>(Person::Age::Young)) {
        cout << "\nСегодняшний вечер подходит для общения с друзьями, "
            "проведения домашних праздников и импровизированных вечеринок. "
            "Будет не только весело, но и интересно: найдётся дело, которое "
            "увлечёт всех.\n";
    }
    else {
        cout << "\nГороскоп для вас находится в разработке. "
            "Приходите чуточку позже ;)\n";
    }
    return 0;
}