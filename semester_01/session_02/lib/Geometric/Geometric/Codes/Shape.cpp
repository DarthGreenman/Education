/* Shape.cpp */

#include "Shape.h"
#include "Exceptions.h"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <numeric>
#include <stdexcept>
#include <typeinfo>

namespace geo {
	Shape::Shape() noexcept :
		p_sides_{}, p_angles_{}
	{
	}

	Shape::Shape(size_t size) try :
		p_sides_{ std::make_unique<std::vector<double>>(size) },
		p_angles_{ std::make_unique<std::vector<double>>(size) }
	{
	}
	catch(...) {
		throw;
	}

	Shape::Shape(const std::vector<double>& sides,
		const std::vector<double>& angles) :
		Shape(sides.size())
	{
		try {
			init_sides(sides);
			init_angles(angles);
		}
		catch (...) {
			throw;
		}
	}

	Shape::Shape(Shape&& movable) noexcept :
		Shape()
	{
		std::swap(p_sides_, movable.p_sides_);
		std::swap(p_angles_, movable.p_angles_);
	}

	Shape::~Shape() {}

	Shape& Shape::operator=(Shape&& movable) noexcept
	{
		if (this != &movable)
		{
			p_sides_.reset();
			p_angles_.reset();

			std::swap(p_sides_, movable.p_sides_);
			std::swap(p_angles_, movable.p_angles_);
		}
		return *this;
	}

	double Shape::get_side(double adjacent_one, double adjacent_two,
		double angle_between)
	{
		using std::pow; using std::sqrt; using std::cos;
		return sqrt(pow(adjacent_one, 2) +
			pow(adjacent_two, 2) -
			2 * adjacent_one * adjacent_two *
			cos(angle_between * std::numbers::pi / 180.0));
	}

	double Shape::get_angle(const std::vector<double>& sides)
	{
		const size_t size{ sides.size() };
		double angle{};
		switch (size) {
		case 3:
			using std::pow; using std::acos;
			angle = acos((pow(sides[0], 2) +
				pow(sides[1], 2) -
				pow(sides[2], 2)) / (2 * sides[0] * sides[1])) *
				180.0 / std::numbers::pi;
			break;
		case 4:
			angle = sides[0] * 180.0 * (sides.size() - 2) /
				std::accumulate(sides.cbegin(), sides.cend(), 0.0);
			break;
		}
		return angle;
	}

	Shape_info Shape::info() const {
		return Shape_info{ {}, is_correct(), *p_sides_, *p_angles_ };
	}

	double Shape::perimeter() const {
		return std::accumulate(
			p_sides_->cbegin(), p_sides_->cend(), 0.0
		);
	}

	void Shape::init_sides(const std::vector<double>& sides)
	{
		const Errors error{
			sides_is_correct(sides)
		};
		switch (error)
		{
		case Errors::bad_value:
			throw Bad_value("Ошибка создания фигуры.\n"
				"Некорректное значение.");
		case Errors::bad_data:
			throw Bad_data("Ошибка создания фигуры.\n"
				"Некорректные данные длин сторон.");
		}
		std::copy(sides.cbegin(), sides.cend(),
			p_sides_->begin());
	}

	void Shape::init_angles(const std::vector<double>& angles)
	{
		const Errors error{
			angles_is_correct(angles) 
		};
		switch (error)
		{
		case Errors::bad_value:
			throw Bad_value("Ошибка создания фигуры.\n"
				"Некорректное значение.");
		case Errors::bad_data:
			throw Bad_data("Ошибка создания фигуры.\n"
				"Некорректные данные углов.");
		}
		std::copy(angles.cbegin(), angles.cend(),
			p_angles_->begin());
	}

	Errors 
		Shape::sides_is_correct(const std::vector<double>& sides) const
	{
		for (const auto& side : sides) {
			if (side <= 0.0) {
				return Errors::bad_value;
			}
		}

		std::vector<double> temp{ sides };
		for (size_t n{}; n < temp.size(); ++n) {
			if (const double sum
				{ std::accumulate(temp.cbegin(), --temp.cend(), 0.0) };
				sum <= temp[temp.size() - 1])
			{
				return Errors::bad_data;
			}
			for (size_t i{}; i < temp.size() - 1; ++i) {
				std::swap(temp[i], temp[i + 1]);
			}
		}
		return Errors::no_errors;
	}

	Errors 
		Shape::angles_is_correct(const std::vector<double>& angles) const {
		for (const auto& angle : angles) {
			if (angle <= 0.0 || angle >= 180.0) {
				return Errors::bad_value;
			}
		}
		return std::abs(sum_angles(angles) -
			180.0 * (angles.size() - 2)) < 0.0001 ? Errors::no_errors :
			Errors::bad_data;
	}

	bool Shape::is_correct() const {
		return sides_is_correct(*p_sides_) == Errors::no_errors && 
			angles_is_correct(*p_angles_) == Errors::no_errors;
	}

	double Shape::sum_angles(const std::vector<double>& angles) const {
		return std::accumulate(
			angles.cbegin(), angles.cend(), 0.0
		);
	}
}