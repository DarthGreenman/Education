/* math_functions.cpp */

#include "math_functions.h"

#include <cmath>

double math::add(double lhs, double rhs) {
    return lhs + rhs;
}

double math::sub(double lhs, double rhs) {
    return lhs - rhs;
}

double math::mul(double lhs, double rhs) {
    return lhs * rhs;
}

std::pair<double, bool> math::div(double divisible, double divider)
{
    return
        divider != 0 ? std::make_pair(divisible / divider, false) :
        std::make_pair(0.0, true);
}

double math::exp(double base, double extent) {
    return std::pow(base, extent);
}
