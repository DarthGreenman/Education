/* Types.h */

#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <string>

namespace geo {
	enum class Form : unsigned {
		Circle = 0, Triangle, Quadrangle,
		Pentagon, Hexagon, Octagon
	};

	enum class Part : unsigned { side = 0, angle, radius };
	enum class Triangle_side : unsigned { ab = 0, bc = 1, ca = 2 };
	enum class Triangle_angle : unsigned { cab = 0, abc = 1, bca = 2 };
	enum class Quadrangle_side : unsigned { ab = 0, bc = 1, cd = 2, da = 3 };
	enum class Quadrangle_angle : unsigned { dab = 0, abc = 1, bcd = 2, cda = 3 };

	struct Param {
		Part feature;
		double value;
	};

	struct Param_list {
		Form form;
		std::vector<Param> params;
	};

	struct Shape_info {
		std::string type;
		bool is_correct;
		std::vector<double> sides;
		std::vector<double> angles;
	};

	struct Triangle_type {
		class Freeform {}; class Isosceles {};
		class Rectangular {}; class Equilateral {};
	};

	struct Quadrangle_type {
		class Freeform {}; class Square {};
		class Rectangle {};	class Parallelogram {}; class Rhombus {};
	};

	/*
	* Класс IShape предоставляет предоставляет интерфейс для всех потомков
	* посредством чистых виртуальных функций, тем самым определяя единообразие
	* методов. В частности: type_info(), perimeter().
	* Могут быть и другие, например: area(), move(...), rotate(...) и т. п.
	*/

	struct IShape {
		virtual void release() = 0;
		virtual Shape_info type_info() const = 0;
		virtual double perimeter() const = 0;
		virtual ~IShape() = 0 {};
	};
}

#endif /* TYPES_H */