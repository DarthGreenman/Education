/* Isosceles.h */

#ifndef TRIANGLE_ISOSCELES_H
#define TRIANGLE_ISOSCELES_H

#include "Triangle.h"

namespace geo {
	class Isosceles :
		public Triangle {
	public:
		Isosceles() = delete;
		Isosceles(double side_one, double side_third)
			: Triangle({ side_one, side_one, side_third }) {
		}
		~Isosceles() {
		}
	
	public:
		void view_info() const override {
			Triangle::view_info();
		}
	};
}
#endif /* TRIANGLE_ISOSCELES_H */
