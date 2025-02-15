/* Rectangular.h */

#ifndef TRIANGLE_RECTANGULAR_H
#define TRIANGLE_RECTANGULAR_H

#include "Triangle.h"

namespace geo {
	class Rectangular :
		public Triangle {
	public:
		using Sides = Triangle::Sides;

		Rectangular() = delete;
		Rectangular(const Sides& sides)
			:Triangle(sides) {
		}
		~Rectangular() {
		}

	public:
		void view_info() const override {
			Triangle::view_info();
		}
	};
}

#endif /* TRIANGLE_RECTANGULAR_H */
