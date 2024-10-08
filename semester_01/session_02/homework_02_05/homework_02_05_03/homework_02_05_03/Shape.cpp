/* Shape.cpp */

#include "Shape.h"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <numeric>
#include <stdexcept>
#include <typeinfo>

namespace geo {
	Shape::Shape(size_t size) try :
		p_sides_{ new std::vector<double>(size) },
		p_angles_{ new std::vector<double>(size) }
	{
	}
	catch(const std::bad_alloc& e) {
		throw e;
	}

	Shape::Shape(const std::vector<double>& sides,
		const std::vector<double>& angles)
		: Shape(sides.size())
	{
		if (!Shape::init_sides(sides) ||
			!Shape::init_angles(angles))
		{
			throw std::runtime_error("Ошибка при создание объекта " +
				std::string(typeid(*this).name()));
		}
	}

	Shape::~Shape()
	{
		if (p_sides_) {
			delete p_sides_;
			p_sides_ = nullptr;
		}

		if (p_angles_) {
			delete p_angles_;
			p_angles_ = nullptr;
		}
	}

	bool Shape::init_sides(const std::vector<double>& sides)
	{
		std::copy(sides.cbegin(), sides.cend(),
			p_sides_->begin());
		return sides_is_correct();
	}

	bool Shape::init_angles(const std::vector<double>& angles)
	{
		std::copy(angles.cbegin(), angles.cend(),
			p_angles_->begin());
		return angles_is_correct();
	}

	Shape_info Shape::info() const {
		return Shape_info{ {}, is_correct(), *p_sides_, *p_angles_ };
	}

	double Shape::perimeter() const {
		return std::accumulate(
			p_sides_->cbegin(), p_sides_->cend(), 0.0
		);
	}

	bool Shape::sides_is_correct() const
	{
		for (const auto& side : *p_sides_) {
			if (side <= 0.0) {
				return false;
			}
		}

		std::vector<double> temp{ *p_sides_ };
		for (size_t n{}; n < temp.size(); ++n) {
			if (const double sum
				{ std::accumulate(temp.cbegin(), --temp.cend(), 0.0) };
				sum < temp[temp.size() - 1])
			{
				return false;
			}
			for (size_t i{}; i < temp.size() - 1; ++i) {
				std::swap(temp[i], temp[i + 1]);
			}
		}
		return true;
	}

	bool Shape::angles_is_correct() const {
		for (const auto& angle : *p_angles_) {
			if (angle <= 0.0 || angle >= 180.0) {
				return false;
			}
		}
		return std::abs(sum_angles() -
			180.0 * (p_angles_->size() - 2)) < 0.0001;
	}

	bool Shape::is_correct() const {
		return sides_is_correct() && angles_is_correct();
	}

	double Shape::sum_angles() const {
		return std::accumulate(
			p_angles_->cbegin(), p_angles_->cend(), 0.0
		);
	}
}