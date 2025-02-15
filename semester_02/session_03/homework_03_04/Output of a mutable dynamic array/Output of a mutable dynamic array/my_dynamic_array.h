// my_dynamic_array.h

#ifndef MY_DYNAMIC_ARRAY_H
#define MY_DYNAMIC_ARRAY_H

#include <cstddef>
#include <memory>

namespace my
{
    template<typename T>
    class My_dynamic_array
    {
    public:
        constexpr My_dynamic_array() = default;
        My_dynamic_array(std::size_t size)
            : size_{ size }, data_{ std::make_unique<T[]>(size) } {}
        My_dynamic_array(const My_dynamic_array&) = delete;
        My_dynamic_array(My_dynamic_array&&) noexcept = delete;

        My_dynamic_array& operator=(const My_dynamic_array&) = delete;
        My_dynamic_array& operator=(My_dynamic_array&&) noexcept = delete;
        ~My_dynamic_array() = default;

        T* begin() { return data_.get(); }
        T* end() { return begin() + size(); }

        const T& operator[](std::size_t i) const { return data_[i]; }
        T& operator[](std::size_t i) { return data_[i]; }

        T* get() const { return data_.get(); }
        std::size_t size() const { return size_; }

    private:
        std::size_t size_{};
        std::unique_ptr<T[]> data_{};
    };
}

#endif // !MY_DYNAMIC_ARRAY_H