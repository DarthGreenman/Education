/* Triangle_rectangular.h */

#ifndef TRIANGLE_RECTANGULAR_H
#define TRIANGLE_RECTANGULAR_H

#include "Triangle_freeform.h"

namespace geometry {

	class Triangle_rectangular :
		public Triangle_freeform {
	public:
		using Triangle_freeform::Lines;

		Triangle_rectangular() = delete;
		Triangle_rectangular(const Lines&);
		~Triangle_rectangular();

	public:
		void view_info() const override;
	};
}

#endif /* TRIANGLE_RECTANGULAR_H */
