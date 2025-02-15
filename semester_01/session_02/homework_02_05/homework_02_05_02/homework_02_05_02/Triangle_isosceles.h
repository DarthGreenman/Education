/* Triangle_isosceles.h */

#ifndef TRIANGLE_ISOSCELES_H
#define TRIANGLE_ISOSCELES_H

#include "Triangle_freeform.h"

namespace geometry {
	class Triangle_isosceles :
		public Triangle_freeform {
	public:
		Triangle_isosceles() = delete;
		Triangle_isosceles(double, double);
		~Triangle_isosceles();
	
	public:
		void view_info() const override;
	};
}
#endif /* TRIANGLE_ISOSCELES_H */
