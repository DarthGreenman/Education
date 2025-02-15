/* Triangle_equilateral.h */

#ifndef TRIANGLE_EQUILATERAL_H
#define TRIANGLE_EQUILATERAL_H

#include "Triangle_freeform.h"

namespace geometry {
	class Triangle_equilateral :
		public Triangle_freeform {
	public:
		Triangle_equilateral() = delete;
		Triangle_equilateral(double);
		~Triangle_equilateral();

	public:
		void view_info() const override;
	};
}

#endif /* TRIANGLE_EQUILATERAL_H */