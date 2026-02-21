
#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

namespace wokwi
{
    class button
    {
    public:
        explicit button(uint8_t pin, unsigned long duration_bounce = 50);
        button(const button &) = delete;
        button(button &&) = delete;
        ~button() = default;

        button &operator=(const button &) = delete;
        button &operator=(button &&) = delete;

        void tick();
        bool click() const;

    private:
        unsigned long _timer{};
        unsigned long _duration_bounce{};
        uint8_t _pin{};
        bool _click{};
        bool _state{};
    };
}

#endif // !BUTTON_H