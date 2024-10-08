// homework_02_07_01.cpp :

#include <iostream>

#include "localisation.h"

#define MODE 1

#ifndef MODE
#error Directive MODE is not defined
#endif /* MODE */

namespace bag {
    template <typename T>
    T get_input_value(const std::string&, const std::string & =
        "Введите корректное значение для данного типа."
    );

    template<typename L, typename R>
    auto add(L lhs, R rhs) -> decltype(lhs + rhs);
}
int main()
{
    using Local = bag::Console_localisation;
    const Local local(Local::Code_pages::rus);
    using std::cout;

#if MODE == 0
    cout << "Работаю в режиме тренировки";
#elif MODE == 1
    cout << "Работаю в боевом режиме\n";
    using Value_type = double;
    Value_type lhs{ bag::get_input_value<Value_type>("Введите число 1: ")};
    Value_type rhs{ bag::get_input_value<Value_type>("Введите число 2: ") };
    cout << "Результат сложения: " << bag::add(lhs, rhs);
#else
    cout << "Неизвестный режим\n"
        "Завершение работы";
#endif // MODE == 0

    return 0;
}

namespace bag {
    template <typename T>
    T get_input_value(const std::string& invite, const std::string& alarm)
    {
        while (true) {
            using std::cout;
            using std::cin;
            cout << invite;
            T res{};
            cin >> res;
            if (cin.fail()) {
                cin.clear();
                cin.ignore((std::numeric_limits<std::streamsize>::max)(),
                    '\n');
                cout << alarm << '\n';
            }
            else {
                cin.ignore((std::numeric_limits<std::streamsize>::max)(),
                    '\n');
                return res;
            }
        }
    }

    template<typename L, typename R>
    auto add(L lhs, R rhs) -> decltype(lhs + rhs) {
        return lhs + rhs;
    }
}