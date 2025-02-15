// space.cpp

#include "space.h"

#include <random>
#include <vector>

namespace space {

	bool operator==(const _place& lhs, const _place& rhs)
	{
		return lhs.first == rhs.first &&
			lhs.second == rhs.second;
	}

	bool operator!=(const _place& lhs, const _place& rhs)
	{
		return !(operator==(lhs, rhs));
	}
	/* 
	* √енератор "живых" клеток.
	* —оздает равномерное распределение целочисленных значений
	* в диапазоне.
	*/
	std::vector<_place> make_life(const _place& place, size_t div)
	{
		auto&& [depth, width] = place;
		std::random_device rd, rw;
		std::mt19937 gd(rd()), gw(rw());
		std::uniform_int_distribution<> dd(0, static_cast<int>(depth - 1)),
			dw(0, static_cast<int>(width - 1));
		std::vector<_place> cells{ ((depth - 1) * (width - 1)) / div };
		
		for (size_t i{}; i < cells.size(); ++i) {
			const _place Place{
				static_cast<size_t>(dd(gd)), static_cast<size_t>(dw(rw))
			};
			cells[i] = Place;
		}
		return cells;
	}
}
