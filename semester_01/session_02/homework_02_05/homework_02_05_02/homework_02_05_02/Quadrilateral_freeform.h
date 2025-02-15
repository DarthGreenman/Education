/* Quadrilateral_freeform.h */

/*
* Quadrilateral_square Ч четырЄхугольник, у которого все углы пр€мые и
* все стороны равны.
* Quadrilateral_parallelogram Ч четырЄхугольник, у которого противоположные
* стороны попарно равны и параллельны.
* Quadrilateral_rectangle Ч четырЄхугольник, у которого все углы пр€мые.
* Quadrilateral_rhombus Ч четырЄхугольник, у которого все стороны равны.
*/

#ifndef QUADRILATERAL_FREEFORM_H
#define QUADRILATERAL_FREEFORM_H

#include "Shape.h"

namespace geometry {
	class Quadrilateral_freeform :
		public Shape {
	public:
		struct Lines {
			double ab, bc, cd, da;
		};

	protected:
		struct Angles {
			double dab, abc, bcd, cda;
		};
	
	public:
		Quadrilateral_freeform() = delete;
		Quadrilateral_freeform(const Lines&);
	
	public:
		~Quadrilateral_freeform();

		virtual void view_info() const;

	private:
		bool figure_is_correct() const final;
		void calculate_angles() final;

	protected:
		void angles(const Angles&);

	private:
		Lines sides_{};
		Angles angles_{};
	};
}
#endif /* QUADRILATERAL_H */