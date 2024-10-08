/* Triangle_freeform.cpp */

#include "Triangle_freeform.h"

#include <exception>
#include <cmath>
#include <numbers>
#include <iostream>
#include <typeinfo>
#include <string>

#define NDEBUG

namespace geometry
{
	Triangle_freeform::Triangle_freeform(const Lines& sides)
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

	Triangle_freeform::~Triangle_freeform()
	{
	}

	void Triangle_freeform::view_info() const
	{
		using std::cout;
		cout << "Тип: " << typeid(*this).name();
		cout << "\nСтороны: "
			<< "A = " << sides_.a
			<< ", B = " << sides_.b
			<< ", C = " << sides_.c;
		cout << "\nУглы: "
			<< "Alfa = " << angles_.alfa
			<< " Beta = " << angles_.beta
			<< " Gamma = " << angles_.gamma
			<< '\n';
	}

	bool Triangle_freeform::figure_is_correct() const
	{
		/*
		* Неравенство треугольника в геометрии, функциональном анализе и
		* смежных дисциплинах — это одно из интуитивных свойств расстояния.
		* Оно утверждает, что длина любой стороны треугольника всегда меньше
		* суммы длин двух его других сторон.
		*/
		return sides_.a < sides_.b + sides_.c &&
			sides_.b < sides_.a + sides_.c &&
			sides_.c < sides_.a + sides_.b;
	}

	void Triangle_freeform::calculate_angles()
	{
		/*
		* cos(α) = (a² + c² - b²) / 2ac
		* cos(β) = (a² + b² - c²) / 2ab
		* cos(γ) = (b² + c² - a²) / 2cb
		*/
		angles_.alfa = 
			std::acos((std::pow(sides_.a, 2.0) + std::pow(sides_.c, 2.0) -
			std::pow(sides_.b, 2.0)) / (2.0 * sides_.a * sides_.c)) * 
			180.0 /	std::numbers::pi;

		angles_.beta = 
			std::acos((std::pow(sides_.a, 2.0) + std::pow(sides_.b, 2.0) -
			std::pow(sides_.c, 2.0)) / (2.0 * sides_.a * sides_.b)) *
			180.0 / std::numbers::pi;

		angles_.gamma = 
			std::acos((std::pow(sides_.b, 2.0) + std::pow(sides_.c, 2.0) -
			std::pow(sides_.a, 2.0)) / (2.0 * sides_.c * sides_.b)) *
			180.0 / std::numbers::pi;
#ifdef DEBUG
		const double sum_angles{ angles_.alfa + angles_.beta + angles_.gamma };
#endif /* DEBUG */
	}
}