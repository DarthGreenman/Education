/* quadrilateral.h */

#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H

#include "shape.h"

namespace geo {
	class Quadrilateral :
		public Shape {
	public:
		Quadrilateral() {
		}

		~Quadrilateral() {
		}

		unsigned number_sides() const override {
			return number_sides_;
		}

	private:
		unsigned number_sides_{ 4 };
	};
}
#endif /* QUADRILATERAL_H */