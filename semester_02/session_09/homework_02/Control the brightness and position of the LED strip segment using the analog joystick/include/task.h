// task.h

#ifndef TASK_H
#define TASK_H

#include <Arduino.h>
#include <stdint.h>

namespace wokwi
{
    template <typename ValueType>
    class task
    {
    public:
        using const_pointer_to_function_type = void (ValueType::*const)();
        using const_pointer_to_object_type = ValueType *const;

        constexpr task() = default;
        explicit task(const_pointer_to_function_type function, const_pointer_to_object_type object,
                      uint32_t polling_time_interval, uint32_t time_before_start = 0ul);
        explicit task(const_pointer_to_function_type function, ValueType &&object, uint32_t polling_time_interval,
                      uint32_t time_before_start = 0ul);
        task(const task &) = default;
        task(task &&) = default;
        ~task() = default;

        task &operator=(const task &&) = delete;
        task &operator=(task &&) = delete;

        void exec();

    private:
        bool wework(uint32_t curr_time) const { return _timeslot.to_work == curr_time ? true : false; }

        ValueType _object{};
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
    /**
     * @brief
     * @param function
     * @param object
     * @param polling_time_interval
     * @param time_before_start
     */
    template <typename ValueType>
    inline task<ValueType>::task(const_pointer_to_function_type function, const_pointer_to_object_type object,
                                 uint32_t polling_time_interval, uint32_t time_before_start)
        : _object{ValueType{}}, _cptr_object{object}, _cptr_func{function},
          _timeslot{polling_time_interval, millis() + time_before_start, millis()}, _we_work{}
    {
    }
    /**
     * @brief
     * @param function
     * @param object
     * @param polling_time_interval
     * @param time_before_start
     */
    template <typename ValueType>
    inline task<ValueType>::task(const_pointer_to_function_type function, ValueType &&object, uint32_t polling_time_interval,
                                 uint32_t time_before_start)
        : _object{object}, _cptr_object{&_object}, _cptr_func{function},
          _timeslot{polling_time_interval, millis() + time_before_start, millis()}, _we_work{}
    {
    }
    /**
     * @brief
     */
    template <typename ValueType>
    inline void task<ValueType>::exec()
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

#endif // TASK_H
