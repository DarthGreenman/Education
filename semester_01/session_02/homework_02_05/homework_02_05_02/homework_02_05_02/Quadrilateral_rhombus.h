/* Quadrilateral_rhombus.h */

#ifndef QUADRILATERAL_RHOMBUS_H
#define QUADRILATERAL_RHOMBUS_H

#include "Quadrilateral_freeform.h"

namespace geometry {
	class Quadrilateral_rhombus :
		public Quadrilateral_freeform {
	public:
		using Quadrilateral_freeform::Lines;

		Quadrilateral_rhombus() = delete;
		Quadrilateral_rhombus(double, double);
		~Quadrilateral_rhombus();

	public:
		void view_info() const override;
	};
}
#endif /* QUADRILATERAL_RHOMBUS_H */