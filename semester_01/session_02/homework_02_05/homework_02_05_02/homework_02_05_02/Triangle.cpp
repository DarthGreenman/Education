/* Triangle.cpp */

#include "Triangle.h"

#include <exception>
#include <cmath>
#include <numbers>
#include <iostream>
#include <typeinfo>
#include <string>

#define NDEBUG

namespace geo
{
	Triangle::Triangle(const Sides& sides)
		: sides_{ sides }
	{
		if (!figure_is_correct()) {
			const std::string message{
				"Фигура типа " +
				std::string{ typeid(*this).name() } +
				" не существует!"
			};
			throw std::exception(message.c_str());
		}
		calculate_angles();
	}

	Triangle::~Triangle()
	{
	}

	void Triangle::view_info() const
	{
		using std::cout;
		cout << "Тип: " << typeid(*this).name();
		cout << "\nСтороны: "
			<< "A = " << sides_.ab
			<< ", B = " << sides_.bc
			<< ", C = " << sides_.ca;
		cout << "\nУглы: "
			<< "Alfa = " << angles_.alfa
			<< " Beta = " << angles_.beta
			<< " Gamma = " << angles_.gamma
			<< '\n';
	}

	bool Triangle::figure_is_correct() const
	{
		/*
		* Неравенство треугольника в геометрии, функциональном анализе и
		* смежных дисциплинах — это одно из интуитивных свойств расстояния.
		* Оно утверждает, что длина любой стороны треугольника всегда меньше
		* суммы длин двух его других сторон.
		*/
		return sides_.ab < sides_.bc + sides_.ca &&
			sides_.bc < sides_.ab + sides_.ca &&
			sides_.ca < sides_.ab + sides_.bc;
	}

	void Triangle::calculate_angles()
	{
		/*
		* cos(α) = (a² + c² - b²) / 2ac
		* cos(β) = (a² + b² - c²) / 2ab
		* cos(γ) = (b² + c² - a²) / 2cb
		*/
		angles_.alfa = 
			std::acos((std::pow(sides_.ab, 2.0) + std::pow(sides_.ca, 2.0) -
			std::pow(sides_.bc, 2.0)) / (2.0 * sides_.ab * sides_.ca)) * 
			180.0 /	std::numbers::pi;

		angles_.beta = 
			std::acos((std::pow(sides_.ab, 2.0) + std::pow(sides_.bc, 2.0) -
			std::pow(sides_.ca, 2.0)) / (2.0 * sides_.ab * sides_.bc)) *
			180.0 / std::numbers::pi;

		angles_.gamma = 
			std::acos((std::pow(sides_.bc, 2.0) + std::pow(sides_.ca, 2.0) -
			std::pow(sides_.ab, 2.0)) / (2.0 * sides_.ca * sides_.bc)) *
			180.0 / std::numbers::pi;
#ifdef DEBUG
		const double sum_angles{ angles_.alfa + angles_.beta + angles_.gamma };
#endif /* DEBUG */
	}
}