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
#include "Types.h"

#include <vector>
#include <typeinfo>

namespace geo {
	/* ТРЕУГОЛЬНИК СВОБОДНОЙ ФОРМЫ - КОНСТРУИРОВАНИЕ ПО ТРЕМ СТОРОНАМ */
	template<class T>
	class Triangle_base :
		public IShape,
		protected Shape
	{
	public:
		Triangle_base() = delete;
		Triangle_base(double ab, double bc, double ca) :
			Shape({ ab, bc, ca }, get_angles({ ab, bc, ca })) {}
		Triangle_base(std::vector<double> values) :
			Shape(values, get_angles(values)) {}
		~Triangle_base() {}

	public:
		void release() {
			if (this) {
				this->~Triangle_base();
			}
		}

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
				abc{ Shape::get_angle(
					{
						sides[static_cast<size_t>(Triangle_side::ab)],
						sides[static_cast<size_t>(Triangle_side::bc)],
						sides[static_cast<size_t>(Triangle_side::ca)]
					}
				)
			},
				bca{ Shape::get_angle(
					{
						sides[static_cast<size_t>(Triangle_side::bc)],
						sides[static_cast<size_t>(Triangle_side::ca)],
						sides[static_cast<size_t>(Triangle_side::ab)]
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
	class Triangle_base<Triangle_type::Isosceles> :
		public IShape,
		protected Shape
	{
	public:
		Triangle_base() = delete;
		Triangle_base(double ab, double abc)	:
			Shape({ ab, ab, Shape::get_side(ab, ab, abc) },
				{ 0.5 * (180.0 - abc), abc, 0.5 * (180.0 - abc) }) {}
		Triangle_base(std::vector<double> values) :
			Shape({ values[0], values[0], Shape::get_side(values[0], values[0], values[1])},
				{ 0.5 * (180.0 - values[1]), values[1], 0.5 * (180.0 - values[1])}) {}
		~Triangle_base() {}

	public:
		void release() {
			if (this) {
				this->~Triangle_base();
			}
		}

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
	class Triangle_base<Triangle_type::Rectangular> :
		public IShape,
		protected Shape
	{
	public:
		Triangle_base() = delete;
		Triangle_base(double ab, double ca) :
			Shape({ ab, Shape::get_side(ab, ca, 90.0), ca },
				get_angles({ ab, Shape::get_side(ab, ca, 90.0), ca })) {}
		Triangle_base(std::vector<double> values) :
			Shape({ values[0], Shape::get_side(values[0], values[1], 90.0), values[1] },
			get_angles({ values[0], Shape::get_side(values[0], values[1], 90.0), values[1] })) {}
		~Triangle_base() {}

	public:
		void release() {
			if (this) {
				this->~Triangle_base();
			}
		}

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
				abc{ Shape::get_angle(
					{
						sides[static_cast<size_t>(Triangle_side::ab)],
						sides[static_cast<size_t>(Triangle_side::bc)],
						sides[static_cast<size_t>(Triangle_side::ca)]
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
	class Triangle_base<Triangle_type::Equilateral> :
		public IShape,
		protected Shape
	{
	public:
		Triangle_base() = delete;
		Triangle_base(double ab) :
			Shape({ ab, ab, ab }, { 180.0 / 3, 180.0 / 3, 180.0 / 3 }) {}
		Triangle_base(std::vector<double> values) :
			Shape({ values[0], values[0], values[0] }, { 180.0 / 3, 180.0 / 3, 180.0 / 3 }) {}
		~Triangle_base() {}

	public:
		void release() {
			if (this) {
				this->~Triangle_base();
			}
		}

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

	using Triangle		= Triangle_base<Triangle_type::Freeform>;
	using Isosceles		= Triangle_base<Triangle_type::Isosceles>;
	using Rectangular	= Triangle_base<Triangle_type::Rectangular>;
	using Equilateral	= Triangle_base<Triangle_type::Equilateral>;
}
#endif /* TRIANGLE_H */