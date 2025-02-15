/* Quadrilateral.h */

#pragma once

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

	template<typename T>
	class Quadrilateral :
		public IShape,
		protected Shape
	{
	public:
		Quadrilateral() = delete;
		Quadrilateral(double ab, double bc, double cd, double da)
			: Shape({ ab, bc, cd, da }, { calculate_angles({ab, bc, cd, da}) })
		{
		};

		~Quadrilateral() {};

	public:
		Shape_info type_info() const override
		{
			auto&& [type, is_correct, sides, angles] = Shape::info();
			type = typeid(*this).name();
			return Shape_info{ type, is_correct, sides, angles };
		};

		double perimeter() const override {
			return Shape::perimeter();
		};

	private:
		std::vector<double> calculate_angles(
			const std::vector<double>& sides) const
		{	
			const double 
				ab{ sides[static_cast<size_t>(Quadrilateral_sides::ab)] },
				bc{	sides[static_cast<size_t>(Quadrilateral_sides::bc)]	},
				cd{	sides[static_cast<size_t>(Quadrilateral_sides::cd)] }, 
				da{	sides[static_cast<size_t>(Quadrilateral_sides::da)] };

			return std::vector<double>{ get_angle({ ab, bc, cd, da }),
					get_angle({ bc, cd, da, ab }),
					get_angle({ cd, da, ab, bc }),
					get_angle({ da, ab, bc, cd })	};
		};
	};

	/* Полная специализация для типа Square ***************************************/
	template<>
	class Quadrilateral<Quadrilateral_types::Square> :
		public IShape,
		protected Shape
	{
	public:
		Quadrilateral() = delete;
		Quadrilateral(double ab)
			: Shape({ ab, ab, ab, ab }, { 90.0, 90.0, 90.0, 90.0 })
		{
		};

		~Quadrilateral() {};

	public:
		Shape_info type_info() const override
		{
			auto&& [type, is_correct, sides, angles] = Shape::info();
			type = typeid(*this).name();
			return Shape_info{ type, is_correct, sides, angles };
		};

		double perimeter() const override {
			return Shape::perimeter();
		};
	};

	/* Полная специализация для типа Rectangle ************************************/
	template<>
	class Quadrilateral<Quadrilateral_types::Rectangle> :
		public IShape,
		protected Shape
	{
	public:
		Quadrilateral() = delete;
		Quadrilateral(double ab, double bc)
			: Shape({ ab, bc, ab, bc }, { 90.0, 90.0, 90.0, 90.0 })
		{
		};

		~Quadrilateral() {
		};

	public:
		Shape_info type_info() const override
		{
			auto&& [type, is_correct, sides, angles] = Shape::info();
			type = typeid(*this).name();
			return Shape_info{ type, is_correct, sides, angles };
		};

		double perimeter() const override {
			return Shape::perimeter();
		};
	};

	/* Полная специализация для типа Parallelogram ********************************/
	template<>
	class Quadrilateral<Quadrilateral_types::Parallelogram> :
		public IShape,
		protected Shape
	{
	public:
		Quadrilateral() = delete;
		Quadrilateral(double ab, double bc, double abc)
			: Shape({ ab, bc, ab, bc }, { 180.0 - abc, abc, 180.0 - abc, abc })
		{
		};

		~Quadrilateral() {};

	public:
		Shape_info type_info() const override
		{
			auto&& [type, is_correct, sides, angles] = Shape::info();
			type = typeid(*this).name();
			return Shape_info{ type, is_correct, sides, angles };
		};

		double perimeter() const override {
			return Shape::perimeter();
		};
	};

	/* Полная специализация для типа Rhombus ********************************/
	template<>
	class Quadrilateral<Quadrilateral_types::Rhombus> :
		public IShape,
		protected Shape
	{
	public:
		Quadrilateral() = delete;
		Quadrilateral(double ab, double abc)
			: Shape({ ab, ab, ab, ab }, { 180.0 - abc, abc, 180.0 - abc, abc })
		{
		};

		~Quadrilateral() {};

	public:
		Shape_info type_info() const override
		{
			auto&& [type, is_correct, sides, angles] = Shape::info();
			type = typeid(*this).name();
			return Shape_info{ type, is_correct, sides, angles };
		};

		double perimeter() const override {
			return Shape::perimeter();
		};
	};
}