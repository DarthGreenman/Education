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
template <uint8_t Number_of_channels>
class light_emitting_diode : public wokwi::electronic_component<Number_of_channels>
{
    static_assert(Number_of_channels == 3u, "Number of channels must be 1 or 3");

  public:
    using basic = wokwi::electronic_component<Number_of_channels>;
    enum class common_pin : uint8_t
    {
        anode,
        cathode
    };

    light_emitting_diode() = default;
    explicit light_emitting_diode(const wokwi::channel_connect_params (&channels)[Number_of_channels], common_pin com);
    light_emitting_diode(const light_emitting_diode &) = default;
    light_emitting_diode(light_emitting_diode &&) = default;
    ~light_emitting_diode() = default;

    light_emitting_diode &operator=(const light_emitting_diode &) = delete;
    light_emitting_diode &operator=(light_emitting_diode &&) = delete;

    void increase_brightness(wokwi::rgb_colour channel);
    void decrease_brightness(wokwi::rgb_colour channel);
    void change_brightness(wokwi::rgb_colour channel, uint16_t signal_value);

  private:
    static constexpr wokwi::rgb_colour _channels[Number_of_channels]{
        wokwi::rgb_colour::red, wokwi::rgb_colour::green, wokwi::rgb_colour::blue};
    common_pin _com{};
};

template <uint8_t Number_of_channels>
inline light_emitting_diode<Number_of_channels>::light_emitting_diode(
    const wokwi::channel_connect_params (&channels)[Number_of_channels], common_pin com)
    : wokwi::electronic_component<Number_of_channels>(channels), _com{com}
{
    for (uint8_t channel_number{}; channel_number < Number_of_channels; ++channel_number)
    {
        if (_com == common_pin::anode)
        {
            channels[channel_number].type == wokwi::signal_type::analog
                ? basic::write(channel_number, static_cast<uint16_t>(wokwi::boundary_values_of_analog_signal::high))
                : basic::write(channel_number, static_cast<uint8_t>(HIGH));
        }
    }
}

template <uint8_t Number_of_channels>
inline void light_emitting_diode<Number_of_channels>::increase_brightness(wokwi::rgb_colour channel)
{
    const auto channel_number = helper::find(_channels, channel);
    const uint8_t signal_value = _com == common_pin::anode ? LOW : HIGH;
    basic::write(channel_number, signal_value);
}

template <uint8_t Number_of_channels>
inline void light_emitting_diode<Number_of_channels>::decrease_brightness(wokwi::rgb_colour channel)
{
    const auto channel_number = helper::find(_channels, channel);
    const uint8_t signal_value = _com == common_pin::anode ? HIGH : LOW;
    basic::write(channel_number, signal_value);
}

template <uint8_t Number_of_channels>
inline void light_emitting_diode<Number_of_channels>::change_brightness(wokwi::rgb_colour channel,
                                                                        uint16_t signal_value)
{
    const auto channel_number = helper::find(_channels, channel);
    basic::write(channel_number, signal_value);
}
// class led : public wokwi::electronic_component<Number_of_channels>

template <> class light_emitting_diode<1u> : public wokwi::electronic_component<1u>
{
  public:
    using basic = wokwi::electronic_component<1u>;

    light_emitting_diode() = default;
    explicit light_emitting_diode(const wokwi::channel_connect_params (&channel)[1u]);
    light_emitting_diode(const light_emitting_diode &) = default;
    light_emitting_diode(light_emitting_diode &&) = default;
    ~light_emitting_diode() = default;

    light_emitting_diode &operator=(const light_emitting_diode &) = default;
    light_emitting_diode &operator=(light_emitting_diode &&) = default;

    void blink();
    void blink(uint16_t signal_value);
    void increase_brightness();
    void decrease_brightness();
    void change_brightness(uint16_t signal_value);

  private:
    static constexpr uint8_t _channel_number{};
};

inline light_emitting_diode<1u>::light_emitting_diode(const wokwi::channel_connect_params (&channel)[1u])
    : wokwi::electronic_component<1u>(channel)
{
}

inline void light_emitting_diode<1u>::blink()
{
    !basic::read(_channel_number) ? increase_brightness() : decrease_brightness();
}

inline void light_emitting_diode<1u>::blink(uint16_t signal_value)
{
    !basic::read(_channel_number)
        ? change_brightness(signal_value)
        : change_brightness(static_cast<uint16_t>(wokwi::boundary_values_of_analog_signal::low));
}

inline void light_emitting_diode<1u>::increase_brightness()
{
    basic::write(_channel_number, digital_signal_values::high);
}

inline void light_emitting_diode<1u>::decrease_brightness()
{
    basic::write(_channel_number, digital_signal_values::low);
}

inline void light_emitting_diode<1u>::change_brightness(uint16_t signal_value)
{
    basic::write(_channel_number, signal_value);
}
// class led<1u> : public wokwi::electronic_component<1u>
} // namespace wokwi

#endif // !LIGHT_EMITTING_DIODE_H
