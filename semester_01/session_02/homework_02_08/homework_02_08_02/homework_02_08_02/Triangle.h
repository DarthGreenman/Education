/* Triangle.h */

/*
* Шаблонные классы определяют метод конструирования определенного
* типа данных посредством предоставления типичного конструктора.
* Например: для конструирования равностороненнего треугольника или
* квадрата необходима длина одной стороны, и т. д.
*/

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.h"
#include "Ishape.h"
#include "Exceptions.h"

#include <vector>
#include <typeinfo>

namespace geo {
	enum class Triangle_sides {
		ab = 0, bc = 1, ca = 2
	};
	enum class Triangle_angles {
		cab = 0, abc = 1, bca = 2
	};
	/* ТРЕУГОЛЬНИК СВОБОДНОЙ ФОРМЫ - КОНСТРУИРОВАНИЕ ПО ТРЕМ СТОРОНАМ */
	template<typename T>
	class Triangle_base :
		public IShape,
		protected Shape
	{
	public:
		Triangle_base() = delete;
		Triangle_base(double ab, double bc, double ca) :
			Shape({ ab, bc, ca }, get_angles({ ab, bc, ca })) {}

		~Triangle_base() {}

	public:
		Shape_info type_info() const override
		{
			auto&& [type, is_correct, sides, angles] = Shape::info();
			type = typeid(*this).name();
			return Shape_info{ type, is_correct, sides, angles };
		}

		double perimeter() const override {
			return Shape::perimeter();
		}

	private:
		std::vector<double> get_angles(
			const std::vector<double>& sides) const
		{
			const double
				abc{ get_angle(
					{
						sides[static_cast<size_t>(Triangle_sides::ab)],
						sides[static_cast<size_t>(Triangle_sides::bc)],
						sides[static_cast<size_t>(Triangle_sides::ca)]
					}
				)
			},
				bca{ get_angle(
					{
						sides[static_cast<size_t>(Triangle_sides::bc)],
						sides[static_cast<size_t>(Triangle_sides::ca)],
						sides[static_cast<size_t>(Triangle_sides::ab)]
					}
				)
			};
			return std::vector<double>{ 180.0 - abc - bca, abc, bca };
		}
	};

	/* ТРЕУГОЛЬНИК РАВНОБЕДРЕННЫЙ - КОНСТРУИРОВАНИЕ ПО СТОРОНЕ И ПРИЛЕГАЮЩЕМУ УГЛУ
	 * УСЛОВИЯ КОНСТРУИРОВАНИЯ: СТОРОНА ПРИЛЕГАЮЩАЯ ЗАДАННОЙ РАВНА ЗАДАННОЙ,
	 * УГЛЫ ПРИ ОСНОВАНИИ РАВНЫ МЕЖДУ СОБОЙ */
	template<>
	class Triangle_base<Triangle_types::Isosceles> :
		public IShape,
		protected Shape
	{
	public:
		Triangle_base() = delete;
		Triangle_base(double ab, double abc)	:
			Shape({ ab, ab, get_side(ab, ab, abc) },
				{ 0.5 * (180.0 - abc), abc, 0.5 * (180.0 - abc) }) {}

		~Triangle_base() {}

	public:
		Shape_info type_info() const override
		{
			auto&& [type, is_correct, sides, angles] = Shape::info();
			type = typeid(*this).name();
			return Shape_info{ type, is_correct, sides, angles };
		}

		double perimeter() const override {
			return Shape::perimeter();
		}
	};

	/* ТРЕУГОЛЬНИК ПРЯМОУГОЛЬНЫЙ - КОНСТРУИРОВАНИЕ ПО ДВУМ СТОРОНАМ
	 * УСЛОВИЯ КОНСТРУИРОВАНИЯ: УГОЛ МЕЖДУ ЗАДАННЫМИ СТОРОНАМИ 90 ГРАД. */
	template<>
	class Triangle_base<Triangle_types::Rectangular> :
		public IShape,
		protected Shape
	{
	public:
		Triangle_base() = delete;
		Triangle_base(double ab, double ca) :
			Shape({ ab, get_side(ab, ca, 90.0), ca },
				get_angles({ ab, get_side(ab, ca, 90.0), ca })) {}

		~Triangle_base() {}

	public:
		Shape_info type_info() const override
		{
			auto&& [type, is_correct, sides, angles] = Shape::info();
			type = typeid(*this).name();
			return Shape_info{ type, is_correct, sides, angles };
		}

		double perimeter() const override {
			return Shape::perimeter();
		}

	private:
		std::vector<double> get_angles(
			const std::vector<double>& sides) const
		{
			const double
				abc{ get_angle(
					{
						sides[static_cast<size_t>(Triangle_sides::ab)],
						sides[static_cast<size_t>(Triangle_sides::bc)],
						sides[static_cast<size_t>(Triangle_sides::ca)]
					}
				)
			};
			return std::vector<double>{ 90.0, abc, 180.0 - 90.0 - abc };
		}
	};

	/* ТРЕУГОЛЬНИК РАВНОСТОРОННИЙ - КОНСТРУИРОВАНИЕ ПО СТОРОНЕ
	 * УСЛОВИЯ КОНСТРУИРОВАНИЯ: СТОРОНЫ РАВНЫ МЕЖДУ СОБОЙ,
	 * УГЛЫ РАВНЫ МЕЖДУ СОБОЙ */
	template<>
	class Triangle_base<Triangle_types::Equilateral> :
		public IShape,
		protected Shape
	{
	public:
		Triangle_base() = delete;
		Triangle_base(double ab) :
			Shape({ ab, ab, ab }, { 180.0 / 3, 180.0 / 3, 180.0 / 3 }) {}

		~Triangle_base() {}

	public:
		Shape_info type_info() const override
		{
			auto&& [type, is_correct, sides, angles] =
				Shape::info();
			type = typeid(*this).name();
			return Shape_info{ type, is_correct, sides, angles };
		}

		double perimeter() const override {
			return Shape::perimeter();
		}
	};

	using Triangle		= Triangle_base<Triangle_types::Freeform>;
	using Isosceles		= Triangle_base<Triangle_types::Isosceles>;
	using Rectangular	= Triangle_base<Triangle_types::Rectangular>;
	using Equilateral	= Triangle_base<Triangle_types::Equilateral>;
}
#endif /* TRIANGLE_H */