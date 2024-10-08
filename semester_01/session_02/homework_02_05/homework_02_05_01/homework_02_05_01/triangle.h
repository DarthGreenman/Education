/* triangle.h */

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"

namespace geo {
	class Triangle :
		public Shape {
	public:
		Triangle() {
		}

		~Triangle() {
		}

		unsigned number_sides() const override {
			return number_sides_;
		}

	private:
		unsigned number_sides_{ 3 };
	};
}
#endif /* TRIANGLE_H */