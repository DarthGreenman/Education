/* Quadrilateral_square.h */

#ifndef QUADRILATERAL_SQUARE_H
#define QUADRILATERAL_SQUARE_H

#include "Quadrilateral_freeform.h"

namespace geometry {
	class Quadrilateral_square :
		public Quadrilateral_freeform {
	public:
		Quadrilateral_square() = delete;
		Quadrilateral_square(double);
		~Quadrilateral_square();

	public:
		void view_info() const override;
	};
}
#endif /* QUADRILATERAL_SQUARE_H */