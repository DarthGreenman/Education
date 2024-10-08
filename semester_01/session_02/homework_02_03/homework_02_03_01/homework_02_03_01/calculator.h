/* calculator.h */

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <utility>

class calculator
{
public:
	enum class operand_order {
		right_left = -1, left_right = 1
	};

	enum class operand_position {
		left = -1, right = 1
	};

	calculator() = delete;
	calculator(double, double);
	~calculator();

	double add() const;
	double multiply() const;
	double subtract(const calculator::operand_order&) const;
	std::pair<double, bool> divide(const calculator::operand_order&) const;

	double get_value(const calculator::operand_position&);

private:
	std::pair<double, bool> divide(double, double) const;

private:
	double lhv_;
	double rhv_;
};

#endif /* CALCULATOR_H */
