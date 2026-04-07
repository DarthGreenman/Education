// task.h

#ifndef TASK_H_FOR_THE_ARDUINO_UNO_PROJECT
#define TASK_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <Arduino.h>
#include <stdint.h>

namespace wokwi
{
    template <typename Type>
    class task
    {
    public:
        using const_pointer_to_function_type = void (Type::*const)();
        using const_pointer_to_object_type = Type *const;

        task() = default;
        explicit task(const_pointer_to_function_type function, const_pointer_to_object_type object,
                      uint32_t polling_time_interval, uint32_t time_before_start = 0ul);
        explicit task(const_pointer_to_function_type function, Type &&object, uint32_t polling_time_interval,
                      uint32_t time_before_start = 0ul);
        task(const task &) = default;
        task(task &&) = default;
        ~task() = default;

        task &operator=(const task &) = default;
        task &operator=(task &&) = default;

        void exec();

    private:
        bool wework(uint32_t curr_time) const { return _timeslot.to_work == curr_time ? true : false; }

        Type _object{};
        const_pointer_to_object_type _cptr_object{};
        const_pointer_to_function_type _cptr_func{};

        struct timeslot_parameters
        {
            uint32_t polling_interval;
            uint32_t to_work;
            uint32_t work;
        };

        timeslot_parameters _timeslot{};
        bool _we_work{};
    };

    template <typename Type>
    inline task<Type>::task(const_pointer_to_function_type function,
                            const_pointer_to_object_type object, uint32_t polling_time_interval, uint32_t time_before_start)
        : _object{Type{}}, _cptr_object{object}, _cptr_func{function},
          _timeslot{polling_time_interval, millis() + time_before_start, millis()}, _we_work{}
    {
    }

    template <typename Type>
    inline task<Type>::task(const_pointer_to_function_type function, Type &&object, uint32_t polling_time_interval,
                            uint32_t time_before_start)
        : _object{object}, _cptr_object{&_object}, _cptr_func{function},
          _timeslot{polling_time_interval, millis() + time_before_start, millis()}, _we_work{}
    {
    }

    template <typename Type>
    inline void task<Type>::exec()
    {
        const auto curr_time = millis();

        if (!_we_work)
            _we_work = wework(curr_time);

        if (_we_work && curr_time - _timeslot.work >= _timeslot.polling_interval)
        {
            _timeslot.work = curr_time;
            (_cptr_object->*_cptr_func)();
        }
    }
} // namespace wokwi

#endif // !TASK_H_FOR_THE_ARDUINO_UNO_PROJECT
