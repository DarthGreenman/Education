#include "counter.h"

namespace math {

	Counter::Counter()
		: Counter{ 1 }
	{
	}
	Counter::Counter(size_t value)
		: value_{ value }
	{
	}
	
	Counter::~Counter()
	{
	}

	Counter& Counter::operator++()
	{
		++value_;
		return *this;
	}

	Counter& Counter::operator--()
	{
		if (value_ > 0) {
			--value_;
		}
		return *this;
	}

	size_t Counter::get_value() const {
		return value_;
	}
}
