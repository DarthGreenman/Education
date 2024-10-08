#include "counter.h"

namespace bag {
	counter::counter()
		: value_{ 1 }
	{
	}
	counter::counter(size_t value)
		: value_{ value }
	{
	}
	
	counter::~counter()
	{
	}
	
	std::pair<bool, bool> counter::repeat_command(const char id)
	{
		bool repeat{ true };
		bool view{};

		switch (id)
		{
			case '+':
				++value_;
				break;
			case '-':
				if (value_ > 0) {
					--value_;
				}
				break;
			case '=':
				view = true;
				break;
			case 'Q':
				repeat = false;
				break;
		}
		return std::pair<bool, bool>(repeat, view);
	}

	size_t counter::get_value() const
	{
		return value_;
	}
}
