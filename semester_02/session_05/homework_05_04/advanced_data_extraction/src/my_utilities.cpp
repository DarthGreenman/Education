// my_utilities.cpp

#include "../includes/my_utilities.h"

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
