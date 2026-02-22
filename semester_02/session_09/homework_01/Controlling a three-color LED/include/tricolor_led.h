/*
 Светодиоды изготавливаются из полупроводниковых материалов.
 Ниже приведены доступные цвета с падением напряжения на диоде.
 ====================================
 Инфракрасный     | ΔU < 1,90
 ------------------------------------
 Красный          | 1,63 < ΔU < 2,03
 ------------------------------------
 Оранжевый        | 2,03 < ΔU < 2,10
 ------------------------------------
 Жёлтый           | 2,10 < ΔU < 2,18
 ------------------------------------
 Зелёный          | 1,90 < ΔU < 4,00
 ------------------------------------
 Сине-зелёный     | 2,48 < ΔU < 3,70
 ------------------------------------
 Синий            | 2,48 < ΔU < 3,70
 ------------------------------------
 Фиолетовый       | 2,76 < ΔU < 4,00
 ------------------------------------
 Пурпурный        | 2,48 < ΔU < 3,70
 ------------------------------------
 Ультрафиолетовый | 3,10 < ΔU < 4,40
 ------------------------------------
 Белый            | ΔU ≈ 3,50
 ====================================
*/
/*
 Для расчета резистора, ограничивающего ток светодиода,
 используется формула: R = (V - Vl) / Il
*/
/*
 Если у светодиода общий катод, то высокий сигнал digitalWrite(pin, HIGH)
 с управляющих пинов будет включать выбранный цвет, а если общий анод - то выключать.
 Соответственно плавное управление яркостью при помощи ШИМ работает по той же логике:
 у общего катода analogWrite(pin, 200) включит цвет почти на полную яркость,
 а у общего анода - почти полностью погасит.
*/

#ifndef TRICOLOR_LED_H
#define TRICOLOR_LED_H

#include <stdint.h>
#include <stddef.h>
#include <Arduino.h>

#include "rgb_color.h"

namespace wokwi
{
    class tricolor_led
    {
    public:
        enum class common_pin : uint8_t
        {
            anode,
            cathode
        };

        explicit tricolor_led(const uint8_t (&pins)[3], common_pin com);
        tricolor_led(const tricolor_led &) = delete;
        tricolor_led(tricolor_led &&) = delete;
        ~tricolor_led() = default;

        tricolor_led &operator=(const tricolor_led &) = delete;
        tricolor_led &operator=(tricolor_led &&) = delete;

        void power_on(wokwi::rgb_color color) const;
        void power_on() const;
        void power_off(wokwi::rgb_color color) const;
        void power_off() const;

    private:
        void construct_connection_diagram(uint8_t pin, common_pin com) const;
        int _voltage_on_com{};
        uint8_t _channel[3];
    };
}

#endif // !TRICOLOR_LED_H
