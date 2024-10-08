// homework_02_05_02.cpp

#include <iostream>
#include <vector>
#include <exception>

#include "Localisation.h"
#include "Shape.h"
#include "Triangle.h"
#include "Equilateral.h"
#include "Isosceles.h"
#include "Rectangular.h"
#include "Quadrilateral.h"
#include "Square.h"
#include "Parallelogram.h"
#include "Rectangle.h"
#include "Rhombus.h"

int main()
{
	bag::Console_localisation localisation(1251);
	using std::cout;
	using std::cerr;

	try {
		const geo::Triangle      triangle{ { 30, 40, 50 } };
		const geo::Rectangular   rectangular{ { 30, 40, 50 } };
		const geo::Isosceles     isosceles{ 20, 10 };
		const geo::Equilateral   equilateral{ 100 };
		const geo::Quadrilateral quadrilateral{ { 10, 20, 30, 40 } };
		const geo::Square        square{ 40 };
		const geo::Parallelogram parallelogram{ 100, 50, 45 };
		const geo::Rectangle     rectangle{ 100, 50 };
		const geo::Rhombus       rhombus{ 100, 45 };

		const geo::Shape* p_tf{ &triangle };
		const geo::Shape* p_tr{ &rectangular };
		const geo::Shape* p_ti{ &isosceles };
		const geo::Shape* p_te{ &equilateral };
		const geo::Shape* p_qf{ &quadrilateral };
		const geo::Shape* p_qs{ &square };
		const geo::Shape* p_qp{ &parallelogram };
		const geo::Shape* p_qr{ &rectangle };
		const geo::Shape* p_qh{ &rhombus };

		const std::vector<const geo::Shape*> list{
			p_tf, p_tr, p_ti, p_te, p_qf, p_qs, p_qp, p_qr, p_qh
		};

		for (const auto& p : list) {
			p->view_info();
			cout << '\n';
		}
	}
	catch (const std::exception& e) {
		cout << e.what();
	}

	return 0;
}