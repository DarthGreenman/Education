/* Shape.h */

/*
* Базовый класс Shape хранит данные экземляров классов потомков и
* выполняет все затратные операции (выделение и освобождение памяти,
* копирование, циклы) при работе с экземплярами.
*/

#pragma once

#include <vector>
#include "Ishape.h"

namespace geo {
	class Shape {
	protected:
		Shape() = delete;
		Shape(size_t);
		Shape(const std::vector<double>&, const std::vector<double>&);
		~Shape();

	protected:
		Shape_info info() const;
		double perimeter() const;

		bool sides_is_correct() const;
		bool angles_is_correct() const;
		bool is_correct() const;

	private:
		bool init_sides(const std::vector<double>&);
		bool init_angles(const std::vector<double>&);
		double sum_angles() const;

	private:
		std::vector<double>* p_sides_{};
		std::vector<double>* p_angles_{};
	};
}
