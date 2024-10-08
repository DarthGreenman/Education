/* Quadrilateral_rectangle.h */

#ifndef QUADRILATERAL_RECTANGLE_H
#define QUADRILATERAL_RECTANGLE_H

#include "Quadrilateral_freeform.h"

namespace geometry {
	class Quadrilateral_rectangle :
		public Quadrilateral_freeform {
	public:
		Quadrilateral_rectangle() = delete;
		Quadrilateral_rectangle(double, double);
		~Quadrilateral_rectangle();

	public:
		void view_info() const override;
	};
}
#endif /* QUADRILATERAL_RECTANGLE_H */