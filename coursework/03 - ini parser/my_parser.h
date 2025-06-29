// my_parser.h

#ifndef MY_PARSER_H
#define MY_PARSER_H

#include "my_numeric.h"
#include "my_types.h"
#include "my_utilities.h"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace ini
{
    class parser
    {
    public:

        using const_iterator = typename std::string::const_iterator;

        parser() = default;
        parser(std::ifstream& file)
        {
            using namespace std;
            using namespace my::ascii;
            for (string line{}, heading{}; getline(file, line); )
            {
                if (const auto& [first, last] = get_range(std::begin(line), std::end(line)); 
                    first != last)
                {
                    if (const auto& [key, val] = get_param(first, last, heading); 
                        !heading.empty() && !key.empty())
                    {
                        records_.emplace(heading, make_pair(key, val));
                    }
                }
            }
        }

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
        auto get_value(const std::string& param, char delim = '.') const
        {
            using namespace std;
            if (const auto q = count(begin(param), end(param), delim); q != 1)
                throw invalid_argument{ "\nException \"Error in request format\" in line: " + 
                to_string(__LINE__) + ", file:\n" + string{ __FILE__ } + '\n'};

            return get_value<Type>(get_pair(param, delim));
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
                    if constexpr (is_same_v<Type, string> )
                    {
                        const string message{ "HEADING: " + lower->first + ", KEY: " + lower->second.first + " = empty" };
                        return (lower->second.second).empty() ? message : lower->second.second;
                    }
                    else
                        return my::to_number<Type>(lower->second.second);                  

            string error_message{ "\nThere is no parameter with name: " + param.second + "\nParameters from this heading " + param.first + ": " };
            for (auto lower = records_.lower_bound(param.first); lower != records_.upper_bound(param.first); ++lower)
                error_message += lower->second.first + ' ';
            throw std::invalid_argument{ error_message + '\n'};
        }
        
    private:
        // Возвращает интервал, где:
        // начало - первый элемент отличный от ' ' или '\t', или first;
        // конец - позиция элемента ';' или '#', или last.
        auto get_range(const_iterator first, const_iterator last) const ->std::pair<const_iterator, const_iterator>
        {
            if (first == last)
                return make_pair(first, last);

            using namespace my::ascii;
            const auto beg = std::find_if_not(first, last, [](char ch) { return ch == space || ch == tab; });
            const auto end = std::find_if(beg != last ? beg : first, last, [](char ch) { return ch == semicolon ||
                ch == number || ch == tab; });

            return std::make_pair(beg != last ? beg : first, end);
        }

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
                    throw invalid_argument{ "\nException \"Unresolved symbol\" in line: " + 
                    to_string(__LINE__) + ", file:\n" + string{ __FILE__ } + '\n'};
                lexeme.push_back(*first);
            }
            
            return lexeme;
        }

        // Возвращает структуру  { КЛЮЧ, ЗНАЧЕНИЕ }, где:
        // КЛЮЧ, ЗНАЧЕНИЕ - данные для построения таблицы параметров
        // Входные данные: начало и конец подстроки, ЗАГОЛОВОК.
        auto get_param(const_iterator first, const_iterator last, std::string& heading) const -> std::pair<std::string, std::string>
        {
            using namespace std;
            using namespace my::ascii;
            using my::is_digit;
            using my::is_letter;
            // Правила на вхождение символов в наименование секции, ключа и значения.
            auto rule_heading = [](char ch) { return is_letter(ch) || is_digit(ch); };
            auto rule_param_key = [](char ch) { return is_letter(ch) || is_digit(ch) || ch == underscore || ch == period; };
            auto rule_param_val = [](char ch) { return is_letter(ch) || is_digit(ch) || ch == underscore || ch == period ||
                ch == slash || ch == percent || ch == asterisk || ch == double_quote || ch == space || ch == colon || ch == backslash  ||
                ch == comma; };

            switch (*first)
            {
            case A: case B: case C: case D: case E: case F: case G: case H: case I: case J: case K: case L: case M:
            case N: case O: case P: case Q: case R: case S: case T: case U: case V: case W: case X: case Y: case Z:
            case a: case b: case c: case d: case e: case f: case g: case h: case i: case j: case k: case l: case m:
            case n: case o: case p: case q: case r: case s: case t: case u: case v: case w: case x: case y: case z:
                if (const auto to_sign = find(first, last, equality_sign); 
                    to_sign != last && !heading.empty())
                    return make_pair(get_lexeme(first, to_sign, rule_param_key), get_lexeme(next(to_sign), last, rule_param_val));
                break;

            case left_square_bracket:
                if (const auto sign_pos = find(first, last, right_square_bracket); sign_pos != last)
                    heading = get_lexeme(next(first), sign_pos, rule_heading);
                break;

            default:
                throw invalid_argument{ "\nException \"Unresolved symbol - the line must start with either a letter or [\" in line: " +
                    to_string(__LINE__) + ", file:\n" + string{ __FILE__ } + '\n' };
            }

            return pair<string, string>{};
        }
        auto get_pair(const std::string& param, char delim) const -> std::pair<std::string, std::string>
        {
            using namespace std;
            istringstream stream{ param };
            const auto values = my::split(stream, delim);
            if (values.size() < 2ull)
                return make_pair(string{}, string{});

            return make_pair(values[0], values[1]);
        }

    private:
        std::multimap<std::string, std::pair<std::string, std::string>> records_{};
    };
}

#endif // !MY_PARSER_H