// my_types.h

#ifndef MY_TYPES_H
#define MY_TYPES_H

#include <iosfwd>
#include <string>

namespace my
{
    namespace ascii
    {
        enum character
        {
            tab = 0b00001001,
            //
            space = 0b00100000,
            exclamation_mark = 0b00100001, // !
            double_quote = 0b00100010, // "
            number = 0b00100011, // #
            dollar_sign = 0b00100100,
            percent = 0b00100101,
            ampersand = 0b00100110,
            single_quote = 0b00100111, // '
            left_parenthesis = 0b00101000, // (
            right_parenthesis = 0b00101001, // )
            asterisk = 0b00101010, // *
            plus = 0b00101011,
            comma = 0b00101100, // ,
            minus = 0b00101101,
            period = 0b00101110, // .
            slash = 0b00101111,
            zero = 0b00110000,
            one = 0b00110001,
            two = 0b00110010,
            three = 0b00110011,
            four = 0b00110100,
            five = 0b00110101,
            six = 0b00110110,
            seven = 0b00110111,
            eight = 0b00111000,
            nine = 0b00111001,
            colon = 0b00111010, // :
            semicolon = 0b00111011, // ;
            less_than = 0b00111100,
            equality_sign = 0b00111101,
            greater_than = 0b00111110,
            question_mark = 0b00111111,
            at_sign = 0b01000000, // @
            A = 0b01000001,
            B = 0b01000010,
            C = 0b01000011,
            D = 0b01000100,
            E = 0b01000101,
            F = 0b01000110,
            G = 0b01000111,
            H = 0b01001000,
            I = 0b01001001,
            J = 0b01001010,
            K = 0b01001011,
            L = 0b01001100,
            M = 0b01001101,
            N = 0b01001110,
            O = 0b01001111,
            P = 0b01010000,
            Q = 0b01010001,
            R = 0b01010010,
            S = 0b01010011,
            T = 0b01010100,
            U = 0b01010101,
            V = 0b01010110,
            W = 0b01010111,
            X = 0b01011000,
            Y = 0b01011001,
            Z = 0b01011010,
            left_square_bracket = 0b01011011, // [
            backslash = 0b01011100,
            right_square_bracket = 0b01011101, // ]
            caret = 0b01011110, // ^
            underscore = 0b01011111, // _
            grave = 0b01100000,
            a = 0b01100001,
            b = 0b01100010,
            c = 0b01100011,
            d = 0b01100100,
            e = 0b01100101,
            f = 0b01100110,
            g = 0b01100111,
            h = 0b01101000,
            i = 0b01101001,
            j = 0b01101010,
            k = 0b01101011,
            l = 0b01101100,
            m = 0b01101101,
            n = 0b01101110,
            o = 0b01101111,
            p = 0b01110000,
            q = 0b01110001,
            r = 0b01110010,
            s = 0b01110011,
            t = 0b01110100,
            u = 0b01110101,
            v = 0b01110110,
            w = 0b01110111,
            x = 0b01111000,
            y = 0b01111001,
            z = 0b01111010,
            left_curly_bracket = 0b01111011, // {
            vertical_bar = 0b01111100, // |
            right_curly_bracket = 0b01111101, // }
            tilde = 0b01111110,
            delete_sign = 0b01111111,
            beg_uppercase = character::A,
            end_uppercase = character::Z,
            beg_lowercase = character::a,
            end_lowercase = character::z,
            beg_digit = character::zero,
            end_digit = character::nine
        };

        std::ostream& operator<<(std::ostream& os, ascii::character ascii_code)
        {
            return (os << std::string{ static_cast<char>(ascii_code) });
        }
    } // namespace ascii

} // namespace my

#endif // !MY_TYPES_H