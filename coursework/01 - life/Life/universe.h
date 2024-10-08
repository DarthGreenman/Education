// universe.h

#ifndef UNIVERSE_H
#define UNIVERSE_H

#include "nucleus.h"

#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace space {

	using std::vector;
	using _place = std::pair<size_t, size_t>;
	using _range = std::pair<size_t, size_t>;
	using _decision = std::tuple<Nucleus::Status, size_t, size_t>;
	using _status = std::tuple<size_t, size_t, size_t>;

	class Universe {
	public:
		enum class View { points, evolution, neighbors };
		using Value_type = vector<vector<Nucleus>>;
		
		Universe() = delete;
		/*
		* —оздает матрицу и инициализирует клетки:
		*  1.  оординатами положени€.
		*  2. —осто€нием: если координаты текущей клетки совпадают с координатами
		*     из списка "живых" клеток, то space::Nucleus::Status::live, в противном
		*     случае space::Nucleus::Status::dead.
		*  3. —писком соседних клеток с шагом 1.
		*/
		Universe(const _place&, vector<_place>&);
		Universe(const Universe&) = delete;
		Universe(Universe&&) = delete;
		~Universe();

		Universe& operator=(const Universe&) = delete;
		Universe& operator=(Universe&&) = delete;
		/*
		* 1. ѕолучает данные о изменени€х состо€ний клеток.
		* 2. –асчитывает количество "живых" клеток.
		* 3. јккумулирует количество итераций (поколений).
		*/
		_status evolution();
		/*
		* ¬ыводит на экран информацию в зависимости от значени€ переменной
		* типа space::Universe::View, если:
		* 1. space::Universe::View::points - разметка пространства.
		* 2. space::Universe::View::evolution - изменение ¬селенной.
		* 3. space::Universe::View::neighbors - распределение соседей.
		*/
		friend void view(const Universe&, Universe::View,
			const std::string&);

	private:
		/*
		* »щет в предварительно отсортированном списке "живых" клеток совпадение
		* с координатами текущей клетки.
		* ѕри совпадении возвращает состо€ние "живой" клетки.
		* ÷икл по списку "живых" клеток проходит один раз. 
		*/
		Nucleus::Status define_status(const _place&, const vector<_place>&) const;
		/*
		* ‘ормирует список соседних клеток согласно полученного диапазона индексов.
		*/
		vector<_place> mark_neighbors(const _place&) const;
		/*
		* ¬ "мЄртвой" клетке, р€дом с которой ровно три "живых", зарождаетс€ жизнь.
		* ≈сли, р€дом с "живой" есть две или три "живых" соседки - клетка продолжает жить.
		* ≈сли соседей меньше двух или больше трЄх, клетка умирает.
		*/
		Nucleus::Status live_or_die(const _place&, const size_t);
		/*
		* 1. —канирует клетки из списка соседних.
		* 2. ѕолучает состо€ние соседених.
		* 3. јкуммулирует количество "живых" клеток.
		*/
		size_t living_neighbors(const _place&) const;
		/*
		* ѕредоставл€ет диапазон индексов соседних клеток.
		*/
		_range range_neighbors(const _range&, size_t) const;
		Value_type* data() const;
		size_t number_of_living() const;
		
	private:
		Value_type* data_;
		size_t number_of_living_;
	};
}
#endif /* UNIVERSE_H */