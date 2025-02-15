/* Triangle_equilateral.h */

#ifndef TRIANGLE_EQUILATERAL_H
#define TRIANGLE_EQUILATERAL_H

#include "Triangle.h"

namespace geo {
	class Equilateral :
		public Triangle {
	public:
		Equilateral() = delete;
		Equilateral(double side)
			: Triangle({ side, side, side }) {
		}
		~Equilateral() {
		}

	public:
		void view_info() const override	{
			Triangle::view_info();
		}
	};
}

#endif /* TRIANGLE_EQUILATERAL_H */