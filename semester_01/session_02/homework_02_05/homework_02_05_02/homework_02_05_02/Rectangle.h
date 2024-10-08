/* Rectangle.h */

#ifndef QUADRILATERAL_RECTANGLE_H
#define QUADRILATERAL_RECTANGLE_H

#include "Quadrilateral.h"

namespace geo {
	class Rectangle :
		public Quadrilateral {
	public:
		Rectangle() = delete;
		Rectangle(double side_one, double side_two)
			: Quadrilateral({ side_one, side_two, side_one, side_two })
		{
			Quadrilateral::angles( { 90.0, 90.0, 90.0, 90.0 } );
		}
		~Rectangle() {
		}

	public:
		void view_info() const override {
			Quadrilateral::view_info();
		}
	};
}
#endif /* QUADRILATERAL_RECTANGLE_H */