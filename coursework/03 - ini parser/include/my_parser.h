// my_parser.h

#ifndef MY_PARSER_H
#define MY_PARSER_H

#include "my_numeric.h"
#include "my_types.h"

#include <iosfwd>
#include <map>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace my
{
    class parser
    {
    public:

        using const_iterator = typename std::string::const_iterator;

        constexpr parser() = default;
        parser(std::ifstream& file);
        parser(const parser&) = delete;
        parser(parser&&) = delete;
        ~parser() = default;

        parser& operator=(const parser&) = delete;
        parser& operator=(parser&&) = delete;

        // Возращает значение ключа в целочисленном типе; в типе числа с плавающей запятой; в строке,
        // если значение отсутствует возвращает "empty" с указанием заголовка и ключа. 
        // Входные данные: запрос - строка формата "ЗАГОЛОВОК:КЛЮЧ", в данном случае разделитель ':'
        template<typename Type,
            typename = std::enable_if_t<std::is_integral_v<Type> || std::is_floating_point_v<Type> || std::is_same_v<Type, std::string>>>
        auto get_value(const std::string& param) const
        {
            using namespace std;
            if (const auto q = count(begin(param), end(param), ascii::colon); q != 1)
                throw invalid_argument{ "\nException \"Error in request format\" in line: " +
                to_string(__LINE__) + ", file:\n" + string{ __FILE__ } };

            return get_value<Type>(get_pair(param, ascii::colon));
        }

        // Возращает значение ключа в целочисленном типе; в типе числа с плавающей запятой; в строке,
        // если значение отсутствует возвращает "empty" с указанием заголовка и ключа. 
        // Входные данные: запрос - структура { ЗАГОЛОВОК, КЛЮЧ }.
        template<typename Type,
            typename = std::enable_if_t<std::is_integral_v<Type> || std::is_floating_point_v<Type> || std::is_same_v<Type, std::string>>>
        auto get_value(std::pair<std::string, std::string> param) const
        {
            using namespace std;
            auto range = records_.equal_range(param.first);
            for (auto& lower = range.first; lower != range.second; ++lower)
                if (lower->second.first == param.second)
                    if constexpr (is_same_v<Type, int>)
                        return my::to_number<Type>(lower->second.second);
                    else
                    {
                        const string message{ "HEADING: " + lower->first + ", KEY: " + lower->second.first + " = empty"};
                        return (lower->second.second).empty() ? message : lower->second.second;
                    }

            string error_message{ "\nThere is no parameter with name: " + param.second + "\nParameters from this heading " + param.first + ": " };
            for (auto lower = records_.lower_bound(param.first); lower != records_.upper_bound(param.first); ++lower)
                error_message += lower->second.first + ' ';
            throw std::invalid_argument{ error_message };
        }

    private:
        // Возвращает интервал, где:
        // начало - первый элемент отличный от ' ' или '\t', или first;
        // конец - позиция элемента ';' или '#', или last.
        auto get_range(const_iterator first, const_iterator last) const->std::pair<const_iterator, const_iterator>;

        // Возвращает лексему, сформированную по правилу; при нарущении правила функция генерирут исключение и информирует
        // о месте генерации.
        // Входные данные: начало и конец подстроки, правило.
        template<typename Condition>
        auto get_lexeme(const_iterator first, const_iterator last, Condition rules) const->std::string
        {
            using namespace std;
            string lexeme{};
            lexeme.reserve(64);
            for (; first != last; ++first)
            {
                if (!rules(*first))
                    throw invalid_argument{ "\nException \"Unresolved symbol\" in line: " + to_string(__LINE__) +
                        ", file:\n" + string{ __FILE__ } };
                lexeme.push_back(*first);
            }
            
            return lexeme;
        }

        // Возвращает структуру  { КЛЮЧ, ЗНАЧЕНИЕ }, где:
        // КЛЮЧ, ЗНАЧЕНИЕ - данные для построения таблицы параметров
        // Входные данные: начало и конец подстроки, ЗАГОЛОВОК.
        auto get_param(const_iterator first, const_iterator last, std::string& heading) const
            ->std::pair<std::string, std::string>;
        
        auto get_pair(const std::string& param, ascii::character delim) const->std::pair<std::string, std::string>;

    private:
        std::multimap<std::string, std::pair<std::string, std::string>> records_{};
    };
}

#endif // !MY_PARSER_H
