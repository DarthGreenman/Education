/* math_functions.h */

#ifndef MATHEMATICAL_FUNCTIONS_H
#define MATHEMATICAL_FUNCTIONS_H

#include <utility>

namespace math {
	enum class Operation : char {
		add = '+', sub = '-', mul = '*', div = '/', exp = '^'
	};

	double add(double, double);
	double sub(double, double);
	double mul(double, double);
	std::pair<double, bool> div(double, double);
	double exp(double, double);
}

#endif /* MATHEMATICAL_FUNCTIONS_H */