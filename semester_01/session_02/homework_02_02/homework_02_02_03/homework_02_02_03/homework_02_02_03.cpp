// homework_02_02_03.cpp

#include <iostream>
#include <string>

namespace bank {
    struct address {
        std::string city_;
        std::string street_;
        int house_number_;
        int apartment_number_;
        int index_;
        address(const std::string& city, const std::string& street,
            int house_number, int apartment_number, int index)
            : city_{ city }, street_{ street }, house_number_{ house_number },
            apartment_number_{ apartment_number }, index_{ index } {
        }
        void view() {
            std::cout << "\nГород: " << city_
                << "\nУлица: " << street_
                << "\nНомер дома: " << house_number_
                << "\nНомер квартиры: " << apartment_number_
                << "\nИндекс: " << index_;
        }
    };
}

int main()
{
    setlocale(LC_ALL, "rus");

    bank::address location_1{ "Москва", "Арбат", 12, 8, 123456 };
    bank::address location_2{ "Ижевск", "Пушкина", 59, 143, 953769 };

    location_1.view();
    std::cout << '\n';
    location_2.view();

    return 0;
}