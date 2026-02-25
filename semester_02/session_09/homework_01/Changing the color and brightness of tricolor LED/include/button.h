
#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

namespace wokwi
{
    class button
    {
    public:
        constexpr button() = default;
        explicit button(uint8_t pin, uint32_t duration_bounce = 50);
        ~button() = default;

        void tick();
        bool click() const;

    private:
        uint32_t _duration_bounce{};
        uint8_t _pin{};
        bool _click{};
        bool _state{};
    };
} // namespace wokwi

#endif // !BUTTON_H