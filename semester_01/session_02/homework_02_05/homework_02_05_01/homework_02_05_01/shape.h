/* shape.h */

#ifndef SHAPE_H
#define SHAPE_H

namespace geo {
	class Shape	{
	public:
		Shape() {
		}

		~Shape() {
		}

		virtual unsigned number_sides() const {
			return number_sides_;
		}

	private:
		unsigned number_sides_{ 0 };
	};
}
#endif /* SHAPE_H */