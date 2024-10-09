// homework_01_10_02_C.cpp

#include <iostream>

#include "\Users\Darth Greenman\Documents\Visual Studio 2022\source\repos\library\localisation.h"


namespace bag {
    class array {
    public:
        array() = delete;
        array(const size_t);
        ~array();

        void print() const;
    
    private:
        size_t size_;
        double* data_{nullptr};
    };
}

int main()
{
    bag::localisation();

    using std::cin;
    using std::cout;

    cout << "Введите размер массива: ";
    size_t size{ 0 };
    cin >> size;

    try {
        bag::array arr{size};
        cout << "Массив: ";
        arr.print();
    }
    catch (const std::exception& e) {
        cout << e.what();
    }
    
    return 0;
}

namespace bag {
    array::array(const size_t size)
        : size_{ size }, data_{ new double[size] {} } {
    }

    array::~array() {
        delete[] data_;
        data_ = nullptr;
    }

    void array::print() const {
        for (size_t i{ 0 }; i < size_; ++i) {
            std::cout << data_[i] << ' ';
        }
    }
}