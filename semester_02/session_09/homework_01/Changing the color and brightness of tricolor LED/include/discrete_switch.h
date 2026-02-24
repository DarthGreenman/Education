
#ifndef DISCRETE_SWITCH_H
#define DISCRETE_SWITCH_H

#include "button.h"
#include <stdint.h>

namespace wokwi
{
    class discrete_switch
    {
    public:
        discrete_switch(uint8_t increase_button, uint8_t decrease_button, int discreteness,
                        unsigned long duration_bounce = 50);
        ~discrete_switch() = default;

        void tick();
        bool inc();
        bool dec();

    private:
        wokwi::button _increase_button{};
        wokwi::button _decrease_button{};
        int _discreteness{};
        int _counter{};
    };
} // namespace wokwi

#endif // !DISCRETE_SWITCH_H