// my_utilities.h

#ifndef MY_UTILITIES_H
#define MY_UTILITIES_H

#include <iosfwd>
#include <sstream>
#include <string>
#include <vector>

namespace my
{
    auto split(std::istringstream& stream, char delim) -> std::vector<std::string>;
}

#endif // !MY_UTILITIES_H