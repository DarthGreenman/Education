/* Square.h */

#ifndef QUADRILATERAL_SQUARE_H
#define QUADRILATERAL_SQUARE_H

#include "Quadrilateral.h"

namespace geo {
	class Square :
		public Quadrilateral {
	public:
		Square() = delete;
		Square(double side)
			: Quadrilateral({ side, side, side, side }) 
		{
			Quadrilateral::angles({ 90.0, 90.0, 90.0, 90.0 });
		}
		~Square() {
		}

	public:
		void view_info() const override	{
			Quadrilateral::view_info();
		}
	};
}
#endif /* QUADRILATERAL_SQUARE_H */