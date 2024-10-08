/* Triangle_freeform.h */

/*
* Triangle_freeform
* Triangle_equilateral
* Triangle_isosceles
* Triangle_rectangular
*/

#ifndef TRIANGLE_FREEFORM_H
#define TRIANGLE_FREEFORM_H

#include "Shape.h"

namespace geometry {
	class Triangle_freeform :
		public Shape {
	public:
		struct Lines {
			double a, b, c;
		};

	protected:
		struct Angles {
			double alfa, beta, gamma;
		};

	public:
		Triangle_freeform() = delete;
		Triangle_freeform(const Lines&);
		~Triangle_freeform();

		void view_info() const override;

	private:
		bool figure_is_correct() const;
		void calculate_angles() final;

	private:
		Lines sides_{};
		Angles angles_{};
	};
}
#endif /* TRIANGLE_FREEFORM_H */