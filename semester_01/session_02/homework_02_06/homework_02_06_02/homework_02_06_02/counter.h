/* counter.h */

#ifndef COUNTER_H
#define COUNTER_H

namespace math {
	class Counter {
	public:
		enum class Operation : char {
			inñ = '+', dec = '-', assign = '='
		};

		Counter();
		Counter(size_t);
		~Counter();

		Counter& operator++();
		Counter& operator--();
		size_t get_value() const;

	private:
		size_t value_{};
	};
}
#endif /* COUNTER_H */