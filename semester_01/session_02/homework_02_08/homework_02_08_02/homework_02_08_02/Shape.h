/* Shape.h */

/*
* Базовый класс Shape хранит данные экземляров классов потомков и
* выполняет все затратные операции (выделение и освобождение памяти,
* копирование, циклы) при работе с экземплярами.
*/

#ifndef SHAPE_H
#define SHAPE_H

#include "Ishape.h"

#include <vector>
#include <memory>

namespace geo {
	enum class Errors : unsigned {
		no_errors = 0,
		bad_value, bad_data
	};

	class Shape {
	protected:
		Shape() = delete;
		Shape(size_t);
		Shape(const std::vector<double>&, const std::vector<double>&);
		~Shape();

	protected:
		Shape_info info() const;
		double perimeter() const;

	private:
		Errors sides_is_correct(const std::vector<double>&) const;
		Errors angles_is_correct(const std::vector<double>&) const;
		bool is_correct() const;
		void init_sides(const std::vector<double>&);
		void init_angles(const std::vector<double>&);
		double sum_angles(const std::vector<double>&) const;

	private:
		std::unique_ptr<std::vector<double>> p_sides_{};
		std::unique_ptr<std::vector<double>> p_angles_{};
	};
}
#endif /* SHAPE_H */