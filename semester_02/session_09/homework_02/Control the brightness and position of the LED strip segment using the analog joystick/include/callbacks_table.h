// callbacks_table.h

#ifndef TABLE_OF_POINTER_TO_CALLBACK_FUNCTION_H_FOR_THE_ARDUINO_UNO_PROJECT
#define TABLE_OF_POINTER_TO_CALLBACK_FUNCTION_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "events.h"

#include <assert.h>
#include <stdint.h>

namespace wokwi
{
    template <typename E>
    class table_of_callback_functions
    {
    public:
        using pointer_to_callback_function = void (*)();
        /**
         * @brief 
         * 
         */
        static constexpr auto number_of_events = static_cast<uint8_t>(E::end);

        constexpr table_of_callback_functions() = default;
        table_of_callback_functions(const table_of_callback_functions &) = default;
        table_of_callback_functions(table_of_callback_functions &&) = default;
        ~table_of_callback_functions() = default;

        table_of_callback_functions &operator=(const table_of_callback_functions &) = delete;
        table_of_callback_functions &operator=(table_of_callback_functions &&) = default;

        void add_handler(E event, pointer_to_callback_function handler);
        pointer_to_callback_function handler(E event) const;

    private:
        pointer_to_callback_function _events[number_of_events]{};
    };

    template <typename E>
    inline void table_of_callback_functions<E>::add_handler(E event, pointer_to_callback_function handler)
    {
        assert(handler && "Handler should be a valid function pointer");
        _events[static_cast<uint8_t>(event)] = handler;
    }
    template <typename E>
    inline typename table_of_callback_functions<E>::pointer_to_callback_function table_of_callback_functions<E>::handler(
        E event) const
    {
        return _events[static_cast<uint8_t>(event)];
    }
} // namespace wokwi
#endif // !TABLE_OF_POINTER_TO_CALLBACK_FUNCTION_H_FOR_THE_ARDUINO_UNO_PROJECT