/* Quadrilateral_parallelogram.h */

#ifndef QUADRILATERAL_PARALLELOGRAM_H
#define QUADRILATERAL_PARALLELOGRAM_H

#include "Quadrilateral_freeform.h"

namespace geometry {
	class Quadrilateral_parallelogram :
		public Quadrilateral_freeform {
	public:
		Quadrilateral_parallelogram() = delete;
		Quadrilateral_parallelogram(double, double, double);
		~Quadrilateral_parallelogram();

	public:
		void view_info() const override;
	};
}
#endif /* QUADRILATERAL_PARALLELOGRAM_H */