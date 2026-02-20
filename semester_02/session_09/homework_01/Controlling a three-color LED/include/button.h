
#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

namespace wokwi
{
    class button
    {
    public:
        explicit button(uint8_t pin, unsigned long timeout = 50);
        button(const button&) = delete;
        button(button&&) = delete;
        ~button() = default;

        button& operator=(const button&) = delete;
        button& operator=(button&&) = delete;
        
        int get_state(int state) const;
        unsigned long get_response_time() const;

    private:
        uint8_t _pin{};
        unsigned long _timeout{};
    };
}

#endif // !BUTTON_H