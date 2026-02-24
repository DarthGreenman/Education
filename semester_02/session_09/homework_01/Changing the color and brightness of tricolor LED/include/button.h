
#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

namespace wokwi
{
    class button
    {
    public:
        constexpr button() = default;
        explicit button(uint8_t pin, unsigned long duration_bounce = 50);
        ~button() = default;

        void tick();
        bool click() const;

    private:
        uint8_t _pin{};
        unsigned long _duration_bounce{};
        unsigned long _timer{};
        bool _click{};
        bool _state{};
    };
} // namespace wokwi

#endif // !BUTTON_H