/* Quadrilateral.h */

/*
* Шаблонные классы определяют метод конструирования определенного
* типа данных посредством предоставления типичного конструктора.
* Например: для конструирования равностороненнего треугольника или
* квадрата необходима длина одной стороны, и т. д.
*/

#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H

#include "Shape.h"
#include "Ishape.h"

#include <vector>
#include <utility>
#include <string>
#include <exception>
#include <iostream>

namespace geo {
	enum class Quadrilateral_sides {
		ab = 0, bc = 1, cd = 2, da = 3
	};
	enum class Quadrilateral_angles {
		dab = 0, abc = 1, bcd = 2, cda = 3
	};

	/* ЧЕТЫРЕХУГОЛЬНИК СВОБОДНОЙ ФОРМЫ - КОНСТРУИРОВАНИЕ ПО ЧЕТЫРЕМ СТОРОНАМ */
	template<typename T>
	class Quadrilateral_base :
		public IShape,
		protected Shape
	{
	public:
		Quadrilateral_base() = delete;
		Quadrilateral_base(double ab, double bc, double cd, double da)
			: Shape({ ab, bc, cd, da }, { calculate_angles({ab, bc, cd, da}) }) {}

		~Quadrilateral_base() {}

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

	private:
		std::vector<double> calculate_angles(
			const std::vector<double>& sides) const
		{
			const double
				ab{ sides[static_cast<size_t>(Quadrilateral_sides::ab)] },
				bc{ sides[static_cast<size_t>(Quadrilateral_sides::bc)] },
				cd{ sides[static_cast<size_t>(Quadrilateral_sides::cd)] },
				da{ sides[static_cast<size_t>(Quadrilateral_sides::da)] };

			return std::vector<double>{ get_angle({ ab, bc, cd, da }),
				get_angle({ bc, cd, da, ab }),
				get_angle({ cd, da, ab, bc }),
				get_angle({ da, ab, bc, cd })	};
		}
	};

	/* КВАДРАТ - КОНСТРУИРОВАНИЕ ПО СТОРОНЕ
	 * УСЛОВИЯ КОНСТРУИРОВАНИЯ: СТОРОНЫ РАВНЫ МЕЖДУ СОБОЙ, УГЛЫ РАВНЫ МЕЖДУ СОБОЙ */
	template<>
	class Quadrilateral_base<Quadrilateral_types::Square> :
		public IShape,
		protected Shape
	{
	public:
		Quadrilateral_base() = delete;
		Quadrilateral_base(double ab)
			: Shape({ ab, ab, ab, ab }, { 90.0, 90.0, 90.0, 90.0 }) {}

		~Quadrilateral_base() {}

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

	/* ПРЯМОУГОЛЬНИК - КОНСТРУИРОВАНИЕ ПО ДВУМ СМЕЖНЫМ СТОРОНАМ
	 * УСЛОВИЯ КОНСТРУИРОВАНИЯ: ПАРАЛЕЛЛЬНЫЕ СТОРОНЫ РАВНЫ МЕЖДУ СОБОЙ, УГЛЫ РАВНЫ МЕЖДУ СОБОЙ */
	template<>
	class Quadrilateral_base<Quadrilateral_types::Rectangle> :
		public IShape,
		protected Shape
	{
	public:
		Quadrilateral_base() = delete;
		Quadrilateral_base(double ab, double bc)
			: Shape({ ab, bc, ab, bc }, { 90.0, 90.0, 90.0, 90.0 }) {}

		~Quadrilateral_base() {}

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

	/* ПАРАЛЛЕЛОГРАММ - КОНСТРУИРОВАНИЕ ПО ДВУМ СМЕЖНЫМ СТОРОНАМ И УГЛУ МЕЖДУ НИМИ
	 * УСЛОВИЯ КОНСТРУИРОВАНИЯ: ПАРАЛЕЛЛЬНЫЕ СТОРОНЫ РАВНЫ МЕЖДУ СОБОЙ,
	 * ПРОТИВОПОЛОЖНЫЕ УГЛЫ РАВНЫ МЕЖДУ СОБОЙ */
	template<>
	class Quadrilateral_base<Quadrilateral_types::Parallelogram> :
		public IShape,
		protected Shape
	{
	public:
		Quadrilateral_base() = delete;
		Quadrilateral_base(double ab, double bc, double abc)
			: Shape({ ab, bc, ab, bc }, { 180.0 - abc, abc, 180.0 - abc, abc }) {}

		~Quadrilateral_base() {}

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

	/* РОМБ - КОНСТРУИРОВАНИЕ ПО СТОРОНЕ И ПРИЛЕГАЮЩЕМУ УГЛУ
	 * УСЛОВИЯ КОНСТРУИРОВАНИЯ: СТОРОНЫ РАВНЫ МЕЖДУ СОБОЙ,
	 * ПРОТИВОПОЛОЖНЫЕ УГЛЫ РАВНЫ МЕЖДУ СОБОЙ */
	template<>
	class Quadrilateral_base<Quadrilateral_types::Rhombus> :
		public IShape,
		protected Shape
	{
	public:
		Quadrilateral_base() = delete;
		Quadrilateral_base(double ab, double abc)
			: Shape({ ab, ab, ab, ab }, { 180.0 - abc, abc, 180.0 - abc, abc }) {}

		~Quadrilateral_base() {}

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

	using Quadrilateral	= Quadrilateral_base<Quadrilateral_types::Freeform>;
	using Square		= Quadrilateral_base<Quadrilateral_types::Square>;
	using Rectangle		= Quadrilateral_base<Quadrilateral_types::Rectangle>;
	using Parallelogram	= Quadrilateral_base<Quadrilateral_types::Parallelogram>;
	using Rhombus		= Quadrilateral_base<Quadrilateral_types::Rhombus>;
}
#endif /* QUADRILATERAL_H */