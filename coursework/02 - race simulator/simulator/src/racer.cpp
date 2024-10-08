/* "racer.cpp */

#include "racer.h"
#include "types.h"
#include "vehicle_land.h"
#include "vehicle_air.h"

#include <stdexcept>
#include <cctype>

namespace race
{
    std::unique_ptr<vehicles::Vehicle> make_racer(char bar_key)
    {
        using namespace vehicles;
        using namespace vehicles::types;

        std::unique_ptr<Vehicle> racer{};
        switch (std::toupper(bar_key))
        {
            case static_cast<char>(ui::Barkey::BAR_01):
                racer = std::make_unique<Vehicle_land<Bactrian<size_t>>>(10.0, 30);
                break;
            case static_cast<char>(ui::Barkey::BAR_02):
                racer = std::make_unique<Vehicle_land<Dromedar<size_t>>>(40.0, 10);
                break;
            case static_cast<char>(ui::Barkey::BAR_03):
                racer = std::make_unique<Vehicle_land<Centaur<size_t>>>(15.0, 8);
                break;
            case static_cast<char>(ui::Barkey::BAR_04):
                racer = std::make_unique<Vehicle_land<Boots<size_t>>>(6.0, 60);
                break;
            case static_cast<char>(ui::Barkey::BAR_05):
                racer = std::make_unique<Vehicle_air<Carpet<double>>>(10.0);
                break;
            case static_cast<char>(ui::Barkey::BAR_06):
                racer = std::make_unique<Vehicle_air<Eagle<double>>>(8.0);
                break;
            case static_cast<char>(ui::Barkey::BAR_07):
                racer = std::make_unique<Vehicle_air<Broom<double>>>(20.0);
                break;
            default:
                throw std::logic_error{ "Выберите корректное значение." };
        }
        return racer;
    }
}