/* Quadrilateral.h */

#ifndef QUADRILATERAL_FREEFORM_H
#define QUADRILATERAL_FREEFORM_H

#include "Shape.h"

namespace geo {
	class Quadrilateral :
		public Shape {
	protected:
		struct Sides {
			double ab, bc, cd, da;
		};

		struct Angles {
			double dab, abc, bcd, cda;
		};
	
	public:
		Quadrilateral() = delete;
		Quadrilateral(const Sides&);
	
	public:
		~Quadrilateral();

		virtual void view_info() const;

	private:
		bool figure_is_correct() const final;
		void calculate_angles() final;

	protected:
		void angles(const Angles&);

	private:
		Sides sides_{};
		Angles angles_{};
	};
}
#endif /* QUADRILATERAL_H */