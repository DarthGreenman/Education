/* Quadrilateral_freeform.h */

/*
* Quadrilateral_square � ��������������, � �������� ��� ���� ������ �
* ��� ������� �����.
* Quadrilateral_parallelogram � ��������������, � �������� ���������������
* ������� ������� ����� � �����������.
* Quadrilateral_rectangle � ��������������, � �������� ��� ���� ������.
* Quadrilateral_rhombus � ��������������, � �������� ��� ������� �����.
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