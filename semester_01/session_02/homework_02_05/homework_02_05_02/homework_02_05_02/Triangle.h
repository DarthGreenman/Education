/* Triangle.h */

#ifndef TRIANGLE_FREEFORM_H
#define TRIANGLE_FREEFORM_H

#include "Shape.h"

namespace geo {
	class Triangle :
		public Shape {
	public:
		struct Sides {
			double ab, bc, ca;
		};

	protected:
		struct Angles {
			double alfa, beta, gamma;
		};

	public:
		Triangle() = delete;
		Triangle(const Sides&);
		~Triangle();

		void view_info() const override;

	private:
		bool figure_is_correct() const;
		void calculate_angles() final;

	private:
		Sides sides_{};
		Angles angles_{};
	};
}
#endif /* TRIANGLE_FREEFORM_H */