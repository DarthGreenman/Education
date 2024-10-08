/* Rhombus.h */

#ifndef QUADRILATERAL_RHOMBUS_H
#define QUADRILATERAL_RHOMBUS_H

#include "Quadrilateral.h"

namespace geo {
	class Rhombus :
		public Quadrilateral {
	public:
		using Quadrilateral::Sides;

		Rhombus() = delete;
		Rhombus(double side, double angle)
			: Quadrilateral({ side, side, side, side })
		{
			Quadrilateral::angles( 
				{ angle, 180.0 - angle, angle, 180.0 - angle } 
			);
		}
		~Rhombus() {
		}

	public:
		void view_info() const override {
			Quadrilateral::view_info();
		}
	};
}
#endif /* QUADRILATERAL_RHOMBUS_H */