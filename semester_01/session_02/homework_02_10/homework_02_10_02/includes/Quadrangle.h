/* Quadrangle.h */

/*
* Шаблонные классы определяют метод конструирования определенного
* типа данных посредством предоставления типичного конструктора.
* Например: для конструирования равностороненнего треугольника или
* квадрата необходима длина одной стороны, и т. д.
*/

#ifndef QUADRANGLE_H
#define QUADRANGLE_H

#include "Shape.h"
#include "Types.h"

#include <vector>
#include <utility>
#include <string>
#include <exception>
#include <iostream>

namespace geo {

	/* ЧЕТЫРЕХУГОЛЬНИК СВОБОДНОЙ ФОРМЫ - КОНСТРУИРОВАНИЕ ПО ЧЕТЫРЕМ СТОРОНАМ */
	template<class T>
	class Quadrangle_base :
		public IShape,
		protected Shape
	{
	public:
		Quadrangle_base() = delete;
		
		Quadrangle_base(double ab, double bc, double cd, double da) :
			Shape({ ab, bc, cd, da }, calculate_angles({ab, bc, cd, da})) {}

		Quadrangle_base(const std::vector<double> values) :
			Shape(values, calculate_angles(values)) {}

		~Quadrangle_base() {}

	public:
		void release() {
			if (this) {
				this->~Quadrangle_base();
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

	private:
		std::vector<double> calculate_angles(
			const std::vector<double>& sides) const
		{
			const double
				ab{ sides[static_cast<size_t>(Quadrangle_side::ab)] },
				bc{ sides[static_cast<size_t>(Quadrangle_side::bc)] },
				cd{ sides[static_cast<size_t>(Quadrangle_side::cd)] },
				da{ sides[static_cast<size_t>(Quadrangle_side::da)] };

			return std::vector<double>{ get_angle({ ab, bc, cd, da }),
				get_angle({ bc, cd, da, ab }),
				get_angle({ cd, da, ab, bc }),
				get_angle({ da, ab, bc, cd })	};
		}
	};

	/* КВАДРАТ - КОНСТРУИРОВАНИЕ ПО СТОРОНЕ
	 * УСЛОВИЯ КОНСТРУИРОВАНИЯ: СТОРОНЫ РАВНЫ МЕЖДУ СОБОЙ, УГЛЫ РАВНЫ МЕЖДУ СОБОЙ */
	template<>
	class Quadrangle_base<Quadrangle_type::Square> :
		public IShape,
		protected Shape
	{
	public:
		Quadrangle_base() = delete;
		
		Quadrangle_base(double ab) :
			Shape({ ab, ab, ab, ab }, { 90.0, 90.0, 90.0, 90.0 }) {}

		Quadrangle_base(const std::vector<double> values) :
			Shape({ values[0], values[0], values[0], values [0]},
				{90.0, 90.0, 90.0, 90.0}) {}

		~Quadrangle_base() {}

	public:
		void release() {
			if (this) {
				this->~Quadrangle_base();
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

	/* ПРЯМОУГОЛЬНИК - КОНСТРУИРОВАНИЕ ПО ДВУМ СМЕЖНЫМ СТОРОНАМ
	 * УСЛОВИЯ КОНСТРУИРОВАНИЯ: ПАРАЛЕЛЛЬНЫЕ СТОРОНЫ РАВНЫ МЕЖДУ СОБОЙ, УГЛЫ РАВНЫ МЕЖДУ СОБОЙ */
	template<>
	class Quadrangle_base<Quadrangle_type::Rectangle> :
		public IShape,
		protected Shape
	{
	public:
		Quadrangle_base() = delete;
		
		Quadrangle_base(double ab, double bc)
			: Shape({ ab, bc, ab, bc }, { 90.0, 90.0, 90.0, 90.0 }) {}

		Quadrangle_base(const std::vector<double> values)
			: Shape({ values[0], values[1], values[0], values[1]},
				{90.0, 90.0, 90.0, 90.0}) {}

		~Quadrangle_base() {}

	public:
		void release() {
			if (this) {
				this->~Quadrangle_base();
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

	/* ПАРАЛЛЕЛОГРАММ - КОНСТРУИРОВАНИЕ ПО ДВУМ СМЕЖНЫМ СТОРОНАМ И УГЛУ МЕЖДУ НИМИ
	 * УСЛОВИЯ КОНСТРУИРОВАНИЯ: ПАРАЛЕЛЛЬНЫЕ СТОРОНЫ РАВНЫ МЕЖДУ СОБОЙ,
	 * ПРОТИВОПОЛОЖНЫЕ УГЛЫ РАВНЫ МЕЖДУ СОБОЙ */
	template<>
	class Quadrangle_base<Quadrangle_type::Parallelogram> :
		public IShape,
		protected Shape
	{
	public:
		Quadrangle_base() = delete;

		Quadrangle_base(double ab, double bc, double abc) :
			Shape({ ab, bc, ab, bc }, { 180.0 - abc, abc, 180.0 - abc, abc }) {}

		Quadrangle_base(const std::vector<double> values) :
			Shape({ values[0], values[1], values[0], values[1] },
				{ 180.0 - values[2], values[2], 180.0 - values[2], values[2] }) {}

		~Quadrangle_base() {}

	public:
		void release() {
			if (this) {
				this->~Quadrangle_base();
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

	/* РОМБ - КОНСТРУИРОВАНИЕ ПО СТОРОНЕ И ПРИЛЕГАЮЩЕМУ УГЛУ
	 * УСЛОВИЯ КОНСТРУИРОВАНИЯ: СТОРОНЫ РАВНЫ МЕЖДУ СОБОЙ,
	 * ПРОТИВОПОЛОЖНЫЕ УГЛЫ РАВНЫ МЕЖДУ СОБОЙ */
	template<>
	class Quadrangle_base<Quadrangle_type::Rhombus> :
		public IShape,
		protected Shape
	{
	public:
		Quadrangle_base() = delete;
		
		Quadrangle_base(double ab, double abc) :
			Shape({ ab, ab, ab, ab }, { 180.0 - abc, abc, 180.0 - abc, abc }) {}

		Quadrangle_base(const std::vector<double> values) :
			Shape({ values[0], values[0], values[0], values[0] },
				{ 180.0 - values[1], values[1], 180.0 - values[1], values[1] }) {}

		~Quadrangle_base() {}

	public:
		void release() {
			if (this) {
				this->~Quadrangle_base();
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

	using Quadrangle	= Quadrangle_base<Quadrangle_type::Freeform>;
	using Square		= Quadrangle_base<Quadrangle_type::Square>;
	using Rectangle		= Quadrangle_base<Quadrangle_type::Rectangle>;
	using Parallelogram	= Quadrangle_base<Quadrangle_type::Parallelogram>;
	using Rhombus		= Quadrangle_base<Quadrangle_type::Rhombus>;
}
#endif /* QUADRANGLE_H */