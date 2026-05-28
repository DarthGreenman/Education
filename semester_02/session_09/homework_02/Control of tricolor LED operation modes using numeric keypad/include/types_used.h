// types_used.h

#ifndef TYPES_USED_H_FOR_THE_ARDUINO_UNO_PROJECT
#define TYPES_USED_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <Arduino.h>

namespace uno {
struct signal_digital {
    enum {
        low,
        high
    };
};
struct signal_analog {
    enum {
        low,
        high = 1023
    };
};

enum class signal_direction : uint8_t {
    input = INPUT,
    input_pullup = INPUT_PULLUP,
    output = OUTPUT
};

template <uint16_t L, uint16_t H>
struct signal {
    static constexpr auto low = L;
    static constexpr auto high = H;
};
using analog = typename uno::signal<0, 1023>;
using digital = typename uno::signal<0, 1>;
using pwm = typename uno::signal<0, 255>;

template <typename T>
struct is_analog {
    static constexpr auto value = fox::is_same<typename T::signal, uno::analog>::value;
};

template <typename T>
struct is_digital {
    static constexpr auto value = fox::is_same<typename T::signal, uno::digital>::value;
};

template <typename T>
struct is_pwm {
    static constexpr auto value = fox::is_same<typename T::signal, uno::pwm>::value;
};

template <typename T>
constexpr bool Is_analog(T value)
{
    return uno::is_analog<T>::value;
}

template <typename T>
constexpr bool Is_digital(T value)
{
    return uno::is_digital<T>::value;
}

template <typename T>
constexpr bool Is_pwm(T value)
{
    return uno::is_pwm<T>::value;
}

namespace color {

    constexpr uint32_t red = 0x00ff0000, green = 0x0000ff00, blue = 0x000000ff;

    template <typename T>
    struct rgb {
        T red;
        T green;
        T blue;
    };

    static rgb<uint8_t> unpack_rgb(uint32_t color)
    {
        return rgb<uint8_t> {
            static_cast<uint8_t>((color >> 16) & 0xff), static_cast<uint8_t>((color >> 8) & 0xff), static_cast<uint8_t>(color & 0xff)
        };
    }

} // namespace color

struct membrane_keypad {
    enum {
        number_of_columns = 4u,
        number_of_rows = 4u
    };
    /**
     * @brief идентификатор измерения таблицы
     */
    enum id {
        col,
        row
    };
    /**
     * @brief в константах перечисления запакованные
     * @brief номера строк и столбцов в старших 4 битах
     * @brief идентификатор измерения таблицы
     */
    enum {
        C1 = id::col << 4 | 0x0, /// 0x00
        C2 = id::col << 4 | 0x1, /// 0x01
        C3 = id::col << 4 | 0x2, /// 0x02
        C4 = id::col << 4 | 0x3, /// 0x03
        R1 = id::row << 4 | 0x0, /// 0x10
        R2 = id::row << 4 | 0x1, /// 0x11
        R3 = id::row << 4 | 0x2, /// 0x12
        R4 = id::row << 4 | 0x3 /// 0x13
    };
    constexpr static auto unpack_num(uint8_t value) -> uint8_t { return static_cast<uint8_t>(value & 0x0f); }
    constexpr static auto unpack_dir(uint8_t value) -> uint8_t { return static_cast<uint8_t>(value >> 4); }

    constexpr static auto pack_pos(uint8_t row_position, uint8_t col_position) -> uint16_t
    {
        return static_cast<uint8_t>(row_position << 4 | col_position);
    }
    constexpr static auto unpack_pos(uint8_t position) -> fox::pair<uint8_t, uint8_t>
    {
        return fox::make_pair(static_cast<uint8_t>(position >> 4), static_cast<uint8_t>(position & 0x0f));
    }
};
} // namespace uno

#endif // !TYPES_USED_H_FOR_THE_ARDUINO_UNO_PROJECT
