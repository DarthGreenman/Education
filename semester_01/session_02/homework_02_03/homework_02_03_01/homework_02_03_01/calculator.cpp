/* calculator.cpp */

#include "calculator.h"

calculator::calculator(double lhv, double rhv) :
	lhv_{ lhv }, rhv_{ rhv } {

}

calculator::~calculator()
{
}

double calculator::add() const
{
	return lhv_ + rhv_;
}

double calculator::multiply() const
{
	return lhv_ * rhv_;
}

double calculator::subtract(const calculator::operand_order& order) const
{
	double result{};

	switch (order)
	{
		case calculator::operand_order::left_right:
			result = lhv_ - rhv_;
			break;
		case calculator::operand_order::right_left:
			result = rhv_ - lhv_;
			break;
	}
	return result;
}

std::pair<double, bool>
calculator::divide(const calculator::operand_order& order) const
{
	std::pair<double, bool> result{};

	switch (order)
	{
		case calculator::operand_order::left_right:
			result = divide(lhv_, rhv_);
			break;
		case calculator::operand_order::right_left:
			result = divide(rhv_, lhv_);
			break;
	}
	return result;
}

double calculator::get_value(const calculator::operand_position& position) {
	return
		position == calculator::operand_position::left ? lhv_ : rhv_;
}

std::pair<double, bool> calculator::divide(double divisible, double divider) const {
	return
		divider != 0 ? std::make_pair(divisible / divider, false) :
		std::make_pair(0.0, true);
}
