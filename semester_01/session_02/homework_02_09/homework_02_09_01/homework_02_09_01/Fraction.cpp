/* Fraction.cpp */

#include "Fraction.h"

namespace bag {

	const bool operator==(const Fraction& lhs, const Fraction& rhs) {
		return lhs.den_ == rhs.den_ ? lhs.num_ == rhs.num_ :
			lhs.num_ * rhs.den_ == rhs.num_ * lhs.den_;
	}

	const bool operator!=(const Fraction& lhs, const Fraction& rhs) {
		return !(lhs == rhs);
	}

	const bool operator<(const Fraction& lhs, const Fraction& rhs) {
		return lhs.den_ == rhs.den_ ? lhs.num_ < rhs.num_ :
			lhs.num_ * rhs.den_ < rhs.num_ * lhs.den_;
	}

	const bool operator>(const Fraction& lhs, const Fraction& rhs) {
		return lhs.den_ == rhs.den_ ? lhs.num_ > rhs.num_ :
			lhs.num_ * rhs.den_ > rhs.num_ * lhs.den_;
	}

	const bool operator<=(const Fraction& lhs, const Fraction& rhs) {
		return lhs < rhs || lhs == rhs;
	}

	const bool operator>=(const Fraction& lhs, const Fraction& rhs) {
		return lhs > rhs || lhs == rhs;
	}

	std::ostream& operator<<(std::ostream& os, const Fraction& fr)
	{
		if (fr.num_ == 0) {
			os << fr.num_;
		}
		else if (fr.num_ == fr.den_) {
			os << fr.num_ / fr.den_;
		}
		else {
			os << fr.num_ << '/' << fr.den_;
		}
		return os;
	}

	Fraction make_fraction(int numerator, int denominator) {
		return Fraction(numerator, denominator);
	}
}