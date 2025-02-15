/* Geometric.cpp */

#include "Geometric.h"
#include "Triangle.h"
#include "Quadrangle.h"
#include "Exceptions.h"

#include <utility>
#include <algorithm>

namespace geo
{
	std::pair<size_t, bool> analyzer_shapes(const Param_list& shape)
	{
		std::vector<Param> params{ shape.params };
		std::sort(params.begin(), params.end(),
			[](Param lhs, Param rhs) {
			return
				static_cast<unsigned>(lhs.feature) < static_cast<unsigned>(rhs.feature);
		}
		);

		bool only_sides{};
		if (params.size() == 1 && params[0].feature == geo::Part::side) {
			only_sides = true;
			return
				std::make_pair(params.size(), only_sides);
		}

		for (size_t i{}; i < params.size() - 1; ++i) {
			const auto curr_part{ static_cast<unsigned>(params[i].feature) };
			const auto next_part{ static_cast<unsigned>(params[i + 1].feature) };
			only_sides =
				curr_part == next_part &&
				params[i + 1].feature == geo::Part::side ? true : false;
		}
		return
			std::make_pair(params.size(), only_sides);
	}

	template<class T, template<class> typename C = Quadrangle_base>
	IShape* make_shape(const std::vector<double> value)
	{
		return new C<T>(value);
	}
	
	IShape* manager_shapes(const Param_list& shape)
	{
		std::vector<double> values{};
		for (const auto& param : shape.params) {
			values.push_back(param.value);
		}
		IShape* p_shape{};

		if (auto&& [num, only_sides] = analyzer_shapes(shape);
			shape.form == Form::Triangle)
		{
			p_shape =
				num == 3 && only_sides ? make_shape<Triangle_type::Freeform, Triangle_base>(values) :
				num == 2 && !only_sides ? make_shape<Triangle_type::Isosceles, Triangle_base>(values) :
				num == 2 && only_sides ? make_shape<Triangle_type::Rectangular, Triangle_base>(values) :
				num == 1 && only_sides ? make_shape<Triangle_type::Equilateral, Triangle_base>(values) :
				nullptr;
		}
		else if (shape.form == Form::Quadrangle) {
			p_shape =
				num == 4 && only_sides ? make_shape < Quadrangle_type::Freeform> (values) :
				num == 3 && !only_sides ? make_shape<Quadrangle_type::Parallelogram>(values) :
				num == 2 && only_sides ? make_shape<Quadrangle_type::Rectangle>(values) :
				num == 2 && !only_sides ? make_shape<Quadrangle_type::Rhombus>(values) :
				num == 1 && only_sides ? make_shape<Quadrangle_type::Square>(values) :
				nullptr;
		}
		else {
			p_shape = nullptr;
		}
		return p_shape ? p_shape : throw Bad_data("Ошибка создания фигуры.\n"
			"Неизвестная фигура.");
	}
	
	IShape* make_shape(const Param_list& shape)
	{
		return manager_shapes(shape);
	}
}