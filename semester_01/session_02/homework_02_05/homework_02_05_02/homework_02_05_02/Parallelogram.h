/* Parallelogram.h */

#ifndef QUADRILATERAL_PARALLELOGRAM_H
#define QUADRILATERAL_PARALLELOGRAM_H

#include "Quadrilateral.h"

namespace geo {
	class Parallelogram :
		public Quadrilateral {
	public:
		Parallelogram() = delete;
		Parallelogram(double first_side, double second_side, double angle)
			: Quadrilateral({ first_side, second_side, first_side, second_side })
		{
			Quadrilateral::angles( 
				{ angle, 180.0 - angle, angle, 180.0 - angle } 
			);
		}
		~Parallelogram() {
		}

	public:
		void view_info() const override {
			Quadrilateral::view_info();
		}
	};
}
#endif /* QUADRILATERAL_PARALLELOGRAM_H */