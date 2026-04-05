// callbacks_table.h

#ifndef TABLE_OF_POINTER_TO_CALLBACK_FUNCTION_H
#define TABLE_OF_POINTER_TO_CALLBACK_FUNCTION_H

#include "events.h"

#include <assert.h>
#include <stdint.h>

namespace wokwi
{
    template <typename Events>
    class table_of_callback_functions
    {
    public:
        using pointer_to_callback_function = void (*)();
        static constexpr auto number_of_events = static_cast<uint8_t>(Events::end);

        constexpr table_of_callback_functions() = default;
        table_of_callback_functions(const table_of_callback_functions &) = default;
        table_of_callback_functions(table_of_callback_functions &&) = default;
        ~table_of_callback_functions() = default;

        table_of_callback_functions &operator=(const table_of_callback_functions &) = delete;
        table_of_callback_functions &operator=(table_of_callback_functions &&) = default;

        void add_handler(Events event, pointer_to_callback_function handler);
        /**
         * @brief
         * @param event
         * @return
         */
        pointer_to_callback_function handler(Events event) const { return _events[static_cast<uint8_t>(event)]; }

    private:
        pointer_to_callback_function _events[number_of_events]{};
    };
    /**
     * @brief
     * @tparam Events
     * @param event
     * @param handler
     */
    template <typename Events>
    inline void table_of_callback_functions<Events>::add_handler(Events event, pointer_to_callback_function handler)
    {
        assert(handler && "Handler should be a valid function pointer");
        _events[static_cast<uint8_t>(event)] = handler;
    }
} // namespace wokwi

#endif // !TABLE_OF_POINTER_TO_CALLBACK_FUNCTION_H