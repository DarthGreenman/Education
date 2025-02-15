// my_dynamic_array.h

#ifndef MY_DYNAMIC_ARRAY_H
#define MY_DYNAMIC_ARRAY_H

#include "my_iterator.h"
#include <memory>

namespace my
{
    template<typename T>
    class My_dynamic_array
    {
    public:
        using value_type = T;
        using size_type = typename std::size_t;
        using pointer = T*;
        using reference = T&;
        using const_reference = const T&;
        using iterator = My_iterator<T>;
        using const_iterator = My_iterator<const T>;

        constexpr My_dynamic_array() = default;
        My_dynamic_array(size_type size, size_type capacity) :
            data_{ std::make_unique<value_type[]>(capacity) },
            first_{ data_.get() }, last_{ first_ + size }, end_{ first_ + capacity } {}
        My_dynamic_array(const My_dynamic_array&) = delete;
        My_dynamic_array(My_dynamic_array&&) noexcept = delete;
        ~My_dynamic_array() = default;

        My_dynamic_array& operator=(const My_dynamic_array&) = delete;
        My_dynamic_array& operator=(My_dynamic_array&&) noexcept = delete;

        auto begin() { return iterator{ first_ }; }
        auto end() { return iterator{ last_ }; }

        auto begin() const { return const_iterator{ first_ }; }
        auto end() const { return const_iterator{ last_ }; }

        auto ñbegin() const { return const_iterator{ first_ }; }
        auto ñend() const { return const_iterator{ last_ }; }

        const_reference operator[](std::size_t i) const { return data_[i]; }
        reference operator[](std::size_t i)
        {
            return const_cast<reference>(static_cast<const My_dynamic_array&>(*this)[i]);
        }

        void push_back(value_type value)
        {
            if (size() == capacity())
                rebuild(capacity() * 2);
            *last_ = value;
            ++last_;
        }

        auto erase()
        {
            if (size() - 1 <= capacity() / 3)
                rebuild(capacity() / 3 + 1);
            shift_left(begin(), end());
            --last_;
            return ñbegin();
        }

        auto size() const { return static_cast<size_type>(last_ - first_); }
        auto capacity() const { return static_cast<size_type>(end_ - first_); }

    private:
        void rebuild(size_type capacity)
        {
            auto tmp = std::make_unique<value_type[]>(capacity);
            const auto size = this->size();
            std::copy(ñbegin(), ñend(), tmp.get());
            data_.reset(new value_type[](capacity));
            std::swap(data_, tmp);
            first_ = data_.get(); last_ = first_ + size; end_ = first_ + capacity;
        }

        void shift_left(iterator first, iterator last)
        {
            for (; first != last; ++first)
                first < last - 1 ? *first = *(first + 1) : *first = value_type{};
        }

    private:
        std::unique_ptr<value_type[]> data_{};
        pointer first_{};
        pointer last_{};
        pointer end_{};
    };
}

#endif // !MY_DYNAMIC_ARRAY_H