// homework_03_04_03.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "array.h"
#include "localisation.h"

#include <iostream>
#include <algorithm>

namespace helper
{
    struct full_size {
        size_t capacity{};
        size_t size{};
    };

    void print_dynamic_array(int* Array, size_t Size, size_t Capacity);
    void print_dynamic_array(const seq::Array<int>& Array);

    enum class answer : char { no = 'N', yes = 'Y' };
}
int main()
{
    const bag::Console_localisation locale{
        bag::Console_localisation::Code_pages::rus
    };

    using std::cout;
    using std::cin;
    helper::full_size size_seq{};
    do {
        cout << "Введите фактичеcкий размер массива: "; cin >> size_seq.capacity;
        cout << "Введите логический размер массива: "; cin >> size_seq.size;
    } while (size_seq.size >= size_seq.capacity);

    seq::Array<int> seq(size_seq.size, size_seq.capacity - size_seq.size);
    std::for_each(seq.begin(), seq.end(),
        [](int& N) {
            static size_t count{};
            std::cout << "Введите arr[" << count++ << "]: ";
            std::cin >> N;
        }
    );

    constexpr std::string_view message{ "\nДинамический массив: " };
    std::cout << message;
    helper::print_dynamic_array(seq.begin().operator->(),
        seq.size(), seq.capacity());

    for (char go_on{ static_cast<char>(helper::answer::yes) };
        std::toupper(go_on) == static_cast<char>(helper::answer::yes); )
    {
        size_t index{};
        cout << "\nВведите индекс элемента, который необходимо удалить: "; cin >> index;
        seq.erase(index);

        std::cout << message;
        helper::print_dynamic_array(seq.begin().operator->(),
            seq.size(), seq.capacity());

        do {
            cout << "\nДля продолжения добавления нажмите Y,\nдля выхода - N: ";
            cin >> go_on;
        } while (std::toupper(go_on) != static_cast<char>(helper::answer::yes) &&
            std::toupper(go_on) != static_cast<char>(helper::answer::no));
    }

    std::cout << "Спасибо!\nВаш массив: ";
    helper::print_dynamic_array(seq.begin().operator->(),
        seq.size(), seq.capacity());

    std::system("pause");
    return 0;
}

namespace helper
{
    void print_dynamic_array(int* Array, size_t Size, size_t Capacity)
    {
        for (size_t i{}; i < Capacity; ++i) {
            i < Size ? std::cout << Array[i] << ' ' : std::cout << "_ ";
        }
    }
    void print_dynamic_array(const seq::Array<int>& Array)
    {
        auto it = Array.cbegin();
        for (size_t i{}, size{ Array.size() }, capacity{ Array.capacity() }; i < capacity; ++i) {
            i < size ? std::cout << *(it++) << ' ' : std::cout << "_ ";
        }
    }
}