// my_utilities.h

#ifndef MY_UTILITIES_H
#define MY_UTILITIES_H

#include <iosfwd>
#include <sstream>
#include <string>
#include <vector>

namespace my
{
    auto split(std::istringstream& stream, char delim) -> std::vector<std::string>
    {
        using namespace std;
        vector<string> lines{};
        lines.reserve(64);
        for (string line; getline(stream, line, delim); )
            lines.emplace_back(line);

        return lines;
    }
}

#endif // !MY_UTILITIES_H