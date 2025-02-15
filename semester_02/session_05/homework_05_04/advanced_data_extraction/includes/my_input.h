/* input.h */

#ifndef INPUT_H
#define INPUT_H

#include <iosfwd>
#include <iostream>
#include <limits>
#include <string>
#include <type_traits>

namespace my
{
    namespace helper
    {
        static auto stream_recovery(std::istream& in, const std::string& alarm =
            "Введите корректное значение для данного типа.")
        {
            using namespace std;
            if (in.fail())
            {
                in.clear();
                in.ignore((numeric_limits<streamsize>::max)(), '\n');
                cout << alarm << '\n'; 
                return false;
            }
            else
            {
                in.ignore((numeric_limits<streamsize>::max)(), '\n');
                return true;
            }
        }
    }

    template<typename Type = std::string,
        typename = std::enable_if_t<std::is_integral_v<Type> || std::is_same_v<Type, std::string>>>
    static auto get_input_value(std::istream& in, Type& value, const std::string& invitation)
    {
        using namespace std;
        for (; cout << invitation, true;)
            if(in >> value; helper::stream_recovery(in))
                return in;
    }

    template<typename Type = std::string,
        typename = std::enable_if_t<std::is_integral_v<Type> || std::is_same_v<Type, std::string>>>
    static auto get_input_value(const std::string& invitation)
    {
        using namespace std;
        for (Type value{}; cout << invitation, true;)
            if (cin >> value; helper::stream_recovery(cin))
                return value;
    }   
}

#endif /* INPUT_H */