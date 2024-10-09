// my_parser.cpp

#include "../include/my_parser.h"
#include "../include/my_types.h"

#include <algorithm>
#include <fstream>
#include <iosfwd>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace my
{
    auto split(std::istringstream& stream, char delim) -> std::vector<std::string>;
}

my::parser::parser(std::ifstream& file)
{
    using namespace std;
    using namespace ascii;
    for (string line{}, heading{}; getline(file, line); )
    {
        if (const auto& [first, last] = get_range(std::begin(line), std::end(line)); first != last)
        {
            if (const auto& [key, val] = get_param(first, last, heading); !heading.empty() && !key.empty())
                records_.emplace(heading, make_pair(key, val));
        }
    }
}

auto my::parser::get_param(const_iterator first, const_iterator last, std::string& heading) const
->std::pair<std::string, std::string>
{
    using namespace std;
    using namespace ascii;
    // Правила на вхождение символов в наименование секции, ключа и значения.
    auto rule_heading = [](char ch) { return is_letter(ch) || is_digit(ch); };
    auto rule_param_key = [](char ch) { return is_letter(ch) || is_digit(ch) || ch == underscore || ch == period; };
    auto rule_param_val = [](char ch) { return is_letter(ch) || is_digit(ch) || ch == underscore || ch == period ||
        ch == slash || ch == percent || ch == asterisk || ch == double_quote || ch == space; };
    
    switch (*first)
    {
        case A: case B: case C: case D: case E: case F: case G: case H: case I: case J: case K: case L: case M:
        case N: case O: case P: case Q: case R: case S: case T: case U: case V: case W: case X: case Y: case Z:
        case a: case b: case c: case d: case e: case f: case g: case h: case i: case j: case k: case l: case m:
        case n: case o: case p: case q: case r: case s: case t: case u: case v: case w: case x: case y: case z:
            if (const auto to_sign = find(first, last, equality_sign); to_sign != last && !heading.empty())
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

auto my::parser::get_range(my::parser::const_iterator first, my::parser::const_iterator last) const
->std::pair<my::parser::const_iterator, my::parser::const_iterator>
{
    if (first == last)
        return make_pair(first, last);
    
    using namespace ascii;
    const auto beg = std::find_if_not(first, last, [](char ch) { return ch == space || ch == tab; });
    const auto end = std::find_if(beg != last ? beg : first, last , [](char ch) { return ch == semicolon ||
        ch == number || ch == space || ch == tab; });
    
    return std::make_pair(beg != last ? beg : first, end);
}


auto my::parser::get_pair(const std::string& param, char delim) const -> std::pair<std::string, std::string>
{
    using namespace std;
    istringstream stream{ param };
    const auto values = split(stream, delim);
    if (values.size() < 2ull)
        return make_pair(string{}, string{});
    
    return make_pair(values[0], values[1]);
}

auto my::split(std::istringstream& stream, char delim) -> std::vector<std::string>
{
    using namespace std;
    vector<string> lines{};
    lines.reserve(64);
    for (string line; getline(stream, line, delim); )
        lines.emplace_back(line);
    
    return lines;
}
