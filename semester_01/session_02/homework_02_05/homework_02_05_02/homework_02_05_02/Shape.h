/* Shape.h */

#ifndef SHAPE_H
#define SHAPE_H

namespace geo {
	class Shape	{
	public:
		virtual void view_info() const = 0;
	
	protected:
		virtual bool figure_is_correct() const = 0;
		virtual void calculate_angles() = 0;
	};
}
#endif /* SHAPE_H */