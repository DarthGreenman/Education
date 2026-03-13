// callbacks_table.h

#ifndef TABLE_OF_POINTER_TO_CALLBACK_FUNCTION_H
#define TABLE_OF_POINTER_TO_CALLBACK_FUNCTION_H

#include <assert.h>
#include <stdint.h>

#include "events.h"

namespace wokwi
{
template <typename Events> class table_of_callback_functions
{
  public:
    using pointer_to_callback_function = void (*)();
    static constexpr auto number_of_events = static_cast<uint8_t>(Events::end);

    constexpr table_of_callback_functions() = default;
    table_of_callback_functions(const table_of_callback_functions &) = default;
    table_of_callback_functions(table_of_callback_functions &&) = default;
    ~table_of_callback_functions() = default;

    table_of_callback_functions &operator=(const table_of_callback_functions &) = delete;
    table_of_callback_functions &operator=(table_of_callback_functions &&) = delete;

    void add_handler(Events event, pointer_to_callback_function handler);
    pointer_to_callback_function handler(Events event) const;

  private:
    pointer_to_callback_function _table_events[number_of_events]{};
};

template <typename Events>
inline void table_of_callback_functions<Events>::add_handler(Events event, pointer_to_callback_function handler)
{
    assert(handler && "Handler should be a valid function pointer");
    _table_events[static_cast<uint8_t>(event)] = handler;
}
template <typename Events>
inline typename table_of_callback_functions<Events>::pointer_to_callback_function table_of_callback_functions<
    Events>::handler(Events event) const
{
    return _table_events[static_cast<uint8_t>(event)];
}

// class table_of_callback_functions
} // namespace wokwi

#endif // !TABLE_OF_POINTER_TO_CALLBACK_FUNCTION_H