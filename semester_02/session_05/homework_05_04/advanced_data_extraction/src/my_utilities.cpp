// my_utilities.cpp

#include "../includes/my_utilities.h"

#include <iosfwd>
#include <string>
#include <vector>

namespace my
{
    auto split(const std::string& line, char delim) -> std::vector<std::string>
    {
        if(line.empty())
            return std::vector<std::string>{};

        using namespace std;
        istringstream stream{ line };
        vector<string> lines{};
        lines.reserve(64);
        for (string line; getline(stream, line, delim); )
            lines.emplace_back(line);

        return lines;
    }
}
