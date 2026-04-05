// led.h

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

#ifndef LIGHT_EMITTING_DIODE_H
#define LIGHT_EMITTING_DIODE_H

#include "hardware.h"
#include "helper.h"
#include "rgb_colour.h"

#include <stdint.h>

namespace wokwi
{
    class light_emitting_diode : public wokwi::electronic_component<wokwi::channel_connect_params>
    {
    public:
        using basic = wokwi::electronic_component<wokwi::channel_connect_params>;
        const static auto channel_number = basic::number_of_channels - 1u;

        constexpr light_emitting_diode() = default;
        explicit light_emitting_diode(uint8_t pin_number, wokwi::signal_type signal_type, uint16_t signal_value)
            : wokwi::electronic_component<wokwi::channel_connect_params>({pin_number, signal_type, wokwi::signal_direction::output})
        {
        }
        light_emitting_diode(const light_emitting_diode &) = default;
        light_emitting_diode(light_emitting_diode &&) = default;
        ~light_emitting_diode() = default;

        light_emitting_diode &operator=(const light_emitting_diode &) = default;
        light_emitting_diode &operator=(light_emitting_diode &&) = default;

        void blink() { !basic::read(channel_number) ? increase_brightness() : decrease_brightness(); }
        void blink(uint16_t signal_value);
        void increase_brightness() { basic::write(channel_number, wokwi::digital_signal_values::high); }
        void decrease_brightness() { basic::write(channel_number, wokwi::digital_signal_values::low); }
        void change_brightness(uint16_t signal_value) { basic::write(channel_number, signal_value); }
    };
    /**
     * @brief
     * @param signal_value
     */
    inline void light_emitting_diode::blink(uint16_t signal_value)
    {
        !basic::read(channel_number) ? change_brightness(signal_value)
                                     : change_brightness(wokwi::analog_signal_low());
    }
} // namespace wokwi

#endif // !LIGHT_EMITTING_DIODE_H
