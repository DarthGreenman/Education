// homework_02_05_04.cpp

#include "localisation.h"
#include "Triangle.h"
#include "Quadrilateral.h"
#include "IShape.h"

#include <iostream>

int main()
{
	bag::Console_localisation localisation(1251);

	using Triangle = geo::Triangle<geo::Triangle_types::Freeform>;
	using Isosceles = geo::Triangle<geo::Triangle_types::Isosceles>;
	using Rectangular = geo::Triangle<geo::Triangle_types::Rectangular>;
	using Equilateral = geo::Triangle<geo::Triangle_types::Equilateral>;

	using Quadrilateral = geo::Quadrilateral<geo::Quadrilateral_types::Freeform>;
	using Square = geo::Quadrilateral<geo::Quadrilateral_types::Square>;
	using Rectangle = geo::Quadrilateral<geo::Quadrilateral_types::Rectangle>;
	using Parallelogram = geo::Quadrilateral<geo::Quadrilateral_types::Parallelogram>;
	using Rhombus = geo::Quadrilateral<geo::Quadrilateral_types::Rhombus>;

	try
	{
		const Triangle triangle{ 30, 50, 40 };
		const Isosceles isosceles{ 60, 120 };
		const Rectangular rectangular{ 30, 40 };
		const Equilateral equilateral{ 250 };

		const Quadrilateral quadrilateral{ 100, 130, 200, 420 };
		const Square square{ 100 };
		const Rectangle rectangle{ 100, 50 };
		const Parallelogram parallelogram{ 100, 150, 30 };
		const Rhombus rhombus{ 100, 110 };

		using Interface_shape = geo::IShape;
		const Interface_shape* p_tr{ &triangle },
			* p_is{ &isosceles },
			* p_rc{ &rectangular },
			* p_eq{ &equilateral },
			* p_qu{ &quadrilateral },
			* p_sq{ &square },
			* p_re{ &rectangle },
			* p_pr{ &parallelogram },
			* p_rh{ &rhombus };

		const std::vector<const Interface_shape*> list{
				p_tr, p_is, p_rc, p_eq, p_qu, p_sq, p_re, p_pr, p_rh
		};

		using std::cout;
		for (const auto& p : list) {
			auto&& [name, is_correct, sides, angles] = p->type_info();
			cout << "\nИмя типа: " << name;

			const std::string message{ is_correct ? "\nФигура корректная." :
				"\nФигура не корректная." };
			cout << message;

			cout << "\nСтороны: ";
			for (const auto& side : sides) {
				cout << side << ' ';
			};

			cout << "\nУглы: ";
			for (const auto& angle : angles) {
				cout << angle << ' ';
			}

			cout << "\nПериметр: " << p->perimeter();
			cout << '\n';
		}
	}
	catch (const std::exception& e) {
		std::cerr << e.what();
	}

	return 0;
}