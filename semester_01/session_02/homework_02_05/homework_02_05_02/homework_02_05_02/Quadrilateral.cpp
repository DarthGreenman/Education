/* Quadrilateral.cpp */

#include "Quadrilateral.h"

#include <exception>
#include <cmath>
#include <numbers>
#include <iostream>
#include <typeinfo>
#include <string>

#define NDEBUG

namespace geo {
	Quadrilateral::Quadrilateral(const Sides& sides)
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

	Quadrilateral::~Quadrilateral()
	{
	}

	void Quadrilateral::view_info() const
	{
		using std::cout;
		cout << "Тип: " << typeid(*this).name();
		cout << "\nСтороны: "
			<< "AB = " << sides_.ab
			<< ", BC = " << sides_.bc
			<< ", CD = " << sides_.cd
			<< ", DA = " << sides_.da;
		cout << "\nУглы: "
			<< "DAB = " << angles_.dab
			<< " ABC = " << angles_.abc
			<< " BCD = " << angles_.bcd
			<< " CDA = " << angles_.cda
			<< '\n';
	}

	bool Quadrilateral::figure_is_correct() const
	{
		/*
		* Каждая сторона четырёхугольника всегда меньше
		* суммы трёх остальных сторон.
		*/
		return sides_.ab < sides_.bc + sides_.cd + sides_.da &&
			sides_.bc < sides_.ab + sides_.cd + sides_.da &&
			sides_.cd < sides_.ab + sides_.bc + sides_.da &&
			sides_.da < sides_.ab + sides_.bc + sides_.cd;
	}

	void Quadrilateral::calculate_angles()
	{
		angles_.dab = sides_.ab * 
			360.0 / (sides_.ab + sides_.bc + sides_.cd + sides_.da);
		angles_.abc = sides_.bc * 
			360.0 / (sides_.ab + sides_.bc + sides_.cd + sides_.da);
		angles_.bcd = sides_.cd * 
			360.0 / (sides_.ab + sides_.bc + sides_.cd + sides_.da);
		angles_.cda = sides_.da * 
			360.0 / (sides_.ab + sides_.bc + sides_.cd + sides_.da);
	#ifdef DEBUG
		const double sum_angles{ angles_.dab + angles_.abc + angles_.bcd +
			angles_.cda };
	#endif /* DEBUG */

	}

	void Quadrilateral::angles(
		const Angles& angles)
	{
		angles_ = angles;
	}
}