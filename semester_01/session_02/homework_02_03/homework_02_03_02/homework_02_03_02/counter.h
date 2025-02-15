/* counter.h */

#ifndef COUNTER_H
#define COUNTER_H

#include <utility>

namespace math {
	class counter {
	public:
		counter();
		counter(size_t);
		~counter();

		std::pair<bool, bool> repeat_command(const char);
		size_t get_value() const;
	private:
		size_t value_{};
	};
}
#endif /* COUNTER_H */