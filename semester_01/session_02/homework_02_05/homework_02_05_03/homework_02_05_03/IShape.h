/* IShape.h */

/*
* Класс IShape предоставляет предоставляет интерфейс для всех потомков
* посредством чистых виртуальных функций, тем самым определяя единообразие
* методов. В частности: type_info(), perimeter().
* Могут быть и другие, например: area(), move(...), rotate(...) и т. п.
*/

#pragma once

#include <vector>
#include <string>

namespace geo {
	struct Shape_info {
		std::string type;
		bool is_correct;
		std::vector<double> sides;
		std::vector<double> angles;
	};

	struct Triangle_types {
		class Freeform {};
		class Isosceles {};
		class Rectangular {};
		class Equilateral {};
	};

	struct Quadrilateral_types {
		class Freeform {};
		class Square {};
		class Rectangle {};
		class Parallelogram {};
		class Rhombus {};
	};

	class IShape {
	public:
		virtual Shape_info type_info() const = 0;
		virtual double perimeter() const = 0;
	};

	double get_side(double, double, double);
	double get_angle(const std::vector<double>&);
}