// universe.cpp

#include "nucleus.h"
#include "universe.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

namespace space {
	/*
	* ������� ������� � �������������� ������:
	*  1. ������������ ���������.
	*  2. ����������: ���� ���������� ������� ������ ��������� � ������������
	*     �� ������ "�����" ������, �� space::Nucleus::Status::live, � ���������
	*     ������ space::Nucleus::Status::dead.
	*  3. ������� �������� ������ � ����� 1.
	*/
	/*
	* ������� �������. 
	* ���������� ��������� ������� Nucleus::Status define_status ���������
	* �������� ������.� ����� ������� ������������� ��������� ����������
	* ����� �������� ������ ���� ���, � ������ ������� �������� "������"
	* ���������� � ������� Nucleus::Status define_status. �� ����� ����������
	* ����� ����������� ������ ��� ��� ������ �������.
	* ������ ����� ���������� ��������� ������������� ������� ����� ���������
	* �����?
	*/
	Universe::Universe(const _place& place, std::vector<_place>& cells)
		: data_{ new std::vector<std::vector<Nucleus>>(place.first, place.second) },
		number_of_living_{}
	{
		std::sort(cells.begin(), cells.end()); /* ������� ������� */
		
		const size_t Depth{ data()->size() },
			Width{ data()->data()->size() };
		
		for (size_t depth{}; depth < Depth; ++depth) {
			for (size_t width{}; width < Width; ++width) {
				Nucleus cell{
					{ depth, width },
					define_status({depth, width}, cells),
					mark_neighbors({ depth, width })
				};
				std::vector<Nucleus>* _cell{ data()->data() };
				_cell[depth][width] = std::move(cell);
				if (_cell[depth][width].status() == Nucleus::Status::live) {
					++number_of_living_;
				}
			}
		}
	}

	Universe::~Universe()
	{
		if (data()) {
			delete data_; data_ = nullptr;
		}
	}
	
	/*
	* 1. �������� ������ � ���������� ��������� ������.
	* 2. ����������� ���������� "�����" ������.
	* 3. ������������ ���������� �������� (���������).
	*/
	_status Universe::evolution()
	{
		std::vector<std::vector<Nucleus>>* world{ data() };
		std::vector<std::vector<Nucleus>>* next =  new std::vector<std::vector<Nucleus>>(
			world->size(), world->data()->size()
		);
		std::copy(world->begin(), world->end(), next->begin()); /* *next = *world; */
		
		static size_t gen{};
		size_t changes{};
		if (!gen) {
			return std::make_tuple(number_of_living(), 0, ++gen);
		}

		const size_t Depth{ next->size() }, Width{ next->data()->size() };
		for (size_t depth{}; depth < Depth; ++depth) {
			for (size_t width{}; width < Width; ++width) {
				const size_t alive{ living_neighbors({ depth, width }) };
				const Nucleus::Status status{ live_or_die({ depth, width }, alive) };
				if (status != world->data()[depth][width].status()) {
					status == Nucleus::Status::live ? ++number_of_living_ :
						--number_of_living_;
					std::vector<Nucleus>* cell{ next->data()};
					cell[depth][width].status(status);
					++changes;
				}
			}
		}
		*world = std::move(*next);
		delete next;
		return std::make_tuple(number_of_living(), changes, ++gen);
	}

	Universe::Value_type* Universe::data() const {
		return data_;
	}

	size_t Universe::number_of_living() const {
		return number_of_living_;
	}

	/*
	* ���� � �������������� ��������������� ������ "�����" ������ ����������
	* � ������������ ������� ������.
	* ��� ���������� ���������� ��������� "�����" ������.
	* ���� �� ������ "�����" ������ �������� ���� ���.
	*/
	Nucleus::Status Universe::define_status(const _place& place,
		const std::vector<_place>& cells) const
	{
		auto&& [depth, width] = place;
		static size_t pos{};
		for (size_t i{ pos }; i < cells.size(); ++i) {
			if (depth == cells[i].first && width == cells[i].second) {
				++pos;
				return Nucleus::Status::live;
			}
		}
		return Nucleus::Status::dead;
	}
	/*
	* ��������� ������ �������� ����� �������� ����������� ��������� ��������.
	*/
	std::vector<_place> Universe::mark_neighbors(const _place& place) const
	{
		const size_t Depth{ data()->size() }, Width{ data()->data()->size() };
		auto&& [_depth, _width] = place;
		std::vector<_place> neighbours {};

		auto&& [beg_depth, end_depth] = range_neighbors({ 0, Depth }, _depth);
		for (auto depth = beg_depth; depth < end_depth; ++depth) {

			auto&& [beg_width, end_width] = range_neighbors({ 0, Width }, _width);
			for (auto width = beg_width; width < end_width; ++width) {
				if (depth != _depth || width != _width) {
					neighbours.push_back({ depth, width });
				}
			}
		}
		return neighbours;
	}
	/*
	* � "������" ������, ����� � ������� ����� ��� "�����", ����������� �����.
	* ����, ����� � "�����" ���� ��� ��� ��� "�����" ������� - ������ ���������� ����.
	* ���� ������� ������ ���� ��� ������ ���, ������ �������.
	*/
	Nucleus::Status Universe::live_or_die(const _place& place, const size_t live)
	{
		const std::vector<Nucleus>* cell{ data()->data() };

		if (!live) {
			return Nucleus::Status::dead;
		}
		auto&& [depth, width] = place;
		if (const size_t correct{ 3 };
			cell[depth][width].is_life()) {
			return live < correct - 1 || live > correct ? Nucleus::Status::dead :
				cell[depth][width].status();
		}
		else {
			return live == correct ? Nucleus::Status::live :
				Nucleus::Status::dead;
		}
	}
	/*
	* 1. ��������� ������ �� ������ ��������.
	* 2. �������� ��������� ���������.
	* 3. ������������ ���������� "�����" ������.
	*/
	size_t Universe::living_neighbors(const _place& place) const
	{
		const std::vector<Nucleus>* cell{ data()->data() };
		const std::vector<_place>* neighbours{
			cell[place.first][place.second].neighbours()
		};

		size_t number_of_living{};
		for (auto p = neighbours->cbegin(); p != neighbours->cend(); ++p) {
			auto&& [depth, width] = *p;
			if (cell[depth][width].is_life()) {
				++number_of_living;
			}
		}
		return number_of_living;
	}
	/*
	* ������������� �������� �������� �������� �����.
	*/
	_range Universe::range_neighbors(const _range& range, size_t pos) const
	{
		const size_t term{ 1 },
			distance{ 1 };
		auto&& [beg, end] = range;

		if (pos == beg) {
			return { beg, beg + distance + term };
		}
		if (pos == end - distance) {
			return { end - distance - term, end };
		}
		return { pos - distance, pos + distance + term };
	}
	/*
	* ������� �� ����� ���������� � ����������� �� �������� ����������
	* ���� space::Universe::View, ����:
	* 1. space::Universe::View::points - �������� ������������.
	* 2. space::Universe::View::evolution - ��������� ���������.
	* 3. space::Universe::View::neighbors - ������������� �������.
	*/
	void view(const Universe& value, Universe::View see,
		const std::string& message)
	{
		using std::cout;
		cout << message << '\n';
		for (auto depth = value.data()->cbegin(); depth != value.data()->cend(); ++depth) {
			for (auto width = depth->cbegin(); width != depth->cend(); ++width) {
				char ch{ width->status() == Nucleus::Status::live ? '*' : ' ' };
				switch (see)
				{
				case space::Universe::View::points:
					ch = '.';
					break;
				case space::Universe::View::evolution:
					/* ��������������� */
					break;
				case space::Universe::View::neighbors:
					ch = width->neighbours()->size() == 3 ? '-' :
						width->neighbours()->size() == 5 ? '+' :
						width->neighbours()->size() == 8 ? 'x' : '?';
					break;
				}
				cout.width(2); cout << ch;
			}
			cout << '\n';
		}
	}
}