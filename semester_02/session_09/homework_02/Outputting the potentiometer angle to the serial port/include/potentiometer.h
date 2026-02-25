// potentiometer.h

#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <stdint.h>

namespace wokwi
{
    template <typename T>
    struct range
    {
        T low;
        T high;
    };

    static constexpr wokwi::range<uint16_t> analog_signal_range{0, 1023};
} // namespace wokwi

namespace wokwi
{
    class potentiometer
    {
    public:
        potentiometer(uint8_t pin, int low, int high, uint32_t data_polling_period);
        potentiometer(const potentiometer &) = delete;
        potentiometer(potentiometer &&) = delete;
        ~potentiometer() = default;

        potentiometer &operator=(const potentiometer &) = delete;
        potentiometer &operator=(potentiometer &&) = delete;

        void tick();
        bool turned() const { return _turned; }
        float angle() const;

    private:
        bool _turned{false};
        uint8_t _pin{};
        wokwi::range<int> _turning_range{};
        int _signal_value{wokwi::analog_signal_range.low};
        uint32_t _data_polling_period{};

    }; // class potentiometer
} // namespace wokwi

#endif // POTENTIOMETER_H