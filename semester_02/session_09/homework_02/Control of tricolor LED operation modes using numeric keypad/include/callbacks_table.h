// callbacks_table.h

#ifndef TABLE_OF_POINTER_TO_CALLBACK_FUNCTION_H_FOR_THE_ARDUINO_UNO_PROJECT
#define TABLE_OF_POINTER_TO_CALLBACK_FUNCTION_H_FOR_THE_ARDUINO_UNO_PROJECT

#include "events.h"

#include <assert.h>
#include <stdint.h>

namespace uno {
template <typename E>
class table_of_callback_functions {
public:
    using pointer_to_callback_function = void (*)();
    /**
     * @brief
     *
     */
    static constexpr auto number_of_events = static_cast<uint8_t>(E::end);

    constexpr table_of_callback_functions() = default;
    table_of_callback_functions(const table_of_callback_functions&) = default;
    table_of_callback_functions(table_of_callback_functions&&) = default;
    ~table_of_callback_functions() = default;

    table_of_callback_functions& operator=(const table_of_callback_functions&) = default;
    table_of_callback_functions& operator=(table_of_callback_functions&&) = default;

    void add(E event, pointer_to_callback_function call)
    {
        assert(call && "Handler should be a valid function pointer");
        _events[static_cast<uint8_t>(event)] = call;
    }
    pointer_to_callback_function call(E event) const { return _events[static_cast<uint8_t>(event)]; }

private:
    pointer_to_callback_function _events[number_of_events] { };
};
} // namespace uno
#endif // !TABLE_OF_POINTER_TO_CALLBACK_FUNCTION_H_FOR_THE_ARDUINO_UNO_PROJECT