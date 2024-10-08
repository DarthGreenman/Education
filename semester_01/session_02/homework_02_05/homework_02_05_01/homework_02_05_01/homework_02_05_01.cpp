// homework_02_05_01.cpp

#include <iostream>

#include "shape.h"
#include "triangle.h"
#include "quadrilateral.h"
#include "localisation.h"

int main()
{
	bag::console_localisation localisation(1251);
	using std::cout;

	cout << "Количество сторон:\n";
	geo::Shape shape;
	cout << typeid(shape).name() << ": " << shape.number_sides() << '\n';
	geo::Triangle triangle;
	cout << typeid(triangle).name() << ": " << triangle.number_sides() << '\n';
	geo::Quadrilateral quadrilateral;
	cout << typeid(quadrilateral).name() << ": " << quadrilateral.number_sides() << '\n';

	return 0;
}