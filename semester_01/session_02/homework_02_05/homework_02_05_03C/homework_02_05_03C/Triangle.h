﻿/* Triangle.h */

#pragma once

#include "Shape.h"
#include "Ishape.h"

#include <vector>
#include <utility>
#include <string>
#include <exception>
#include <iostream>

namespace geo {
	
	enum class Triangle_sides { 
		ab = 0, bc = 1, ca = 2
	};
	enum class Triangle_angles {
		cab = 0, abc = 1, bca = 2
	};

	template<typename T>
	class Triangle :
		public IShape,
		protected Shape
	{
	public:
		Triangle() = delete;		
		Triangle(double ab, double bc, double ca)
			: Shape({ ab, bc, ca }, get_angles({ ab, bc, ca })) 
		{
		};
		
		~Triangle() {};

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
		};
	};

    /* Полная специализация для типа Isosceles **********************************/
	template<>
	class Triangle<Triangle_types::Isosceles> :
		public IShape,
		protected Shape
	{
	public:
		Triangle() = delete;
		Triangle(double ab, double abc)
			: Shape({ ab, ab, get_side(ab, ab, abc)	},
				{ 0.5 * (180.0 - abc), abc, 0.5 * (180.0 - abc) })
		{
		};

		~Triangle() {};

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

	/* Полная специализация для типа Rectangular **********************************/
	template<>
	class Triangle<Triangle_types::Rectangular> :
		public IShape,
		protected Shape
	{
	public:
		Triangle() = delete;
		Triangle(double ab, double ca)
			: Shape({ ab, get_side(ab, ca, 90.0), ca },
				get_angles({ ab, get_side(ab, ca, 90.0), ca }))
		{
		};

		~Triangle() {};

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
		};
	};

	/* Полная специализация для типа Equilateral **********************************/
	template<>
	class Triangle<Triangle_types::Equilateral> :
		public IShape,
		protected Shape
	{
	public:
		Triangle() = delete;
		Triangle(double ab)
			: Shape({ ab, ab, ab },	{ 180.0 / 3, 180.0 / 3, 180.0 / 3 })
		{
		};

		~Triangle() {};

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