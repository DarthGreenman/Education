/* input.h */

#ifndef INPUT_H
#define INPUT_H

#include <__msvc_string_view.hpp>
#include <iosfwd>
#include <iostream>
#include <limits>
#include <type_traits>

namespace my
{
    namespace helper
    {
        auto stream_recovery(std::istream& in, std::string_view alarm =
            "������� ���������� �������� ��� ������� ����.")
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

    template<typename Type,
        typename = std::enable_if_t<std::is_integral_v<Type>>>
    auto get_input_value(std::istream& in, Type& val, std::string_view invitation)
    {
        using namespace std;
        for (; cout << invitation, true;)
            if(in >> val; helper::stream_recovery(in))
                return in;
    }

    template<typename Type,
        typename = std::enable_if_t<std::is_integral_v<Type>>>
    auto get_input_value(std::string_view invitation)
    {
        using namespace std;
        for (Type ret{}; cout << invitation, true;)
            if (cin >> ret; helper::stream_recovery(cin))
                return ret;
    }   
}

#endif /* INPUT_H */