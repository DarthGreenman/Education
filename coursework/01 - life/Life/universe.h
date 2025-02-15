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
		* ������� ������� � �������������� ������:
		*  1. ������������ ���������.
		*  2. ����������: ���� ���������� ������� ������ ��������� � ������������
		*     �� ������ "�����" ������, �� space::Nucleus::Status::live, � ���������
		*     ������ space::Nucleus::Status::dead.
		*  3. ������� �������� ������ � ����� 1.
		*/
		Universe(const _place&, vector<_place>&);
		Universe(const Universe&) = delete;
		Universe(Universe&&) = delete;
		~Universe();

		Universe& operator=(const Universe&) = delete;
		Universe& operator=(Universe&&) = delete;
		/*
		* 1. �������� ������ � ���������� ��������� ������.
		* 2. ����������� ���������� "�����" ������.
		* 3. ������������ ���������� �������� (���������).
		*/
		_status evolution();
		/*
		* ������� �� ����� ���������� � ����������� �� �������� ����������
		* ���� space::Universe::View, ����:
		* 1. space::Universe::View::points - �������� ������������.
		* 2. space::Universe::View::evolution - ��������� ���������.
		* 3. space::Universe::View::neighbors - ������������� �������.
		*/
		friend void view(const Universe&, Universe::View,
			const std::string&);

	private:
		/*
		* ���� � �������������� ��������������� ������ "�����" ������ ����������
		* � ������������ ������� ������.
		* ��� ���������� ���������� ��������� "�����" ������.
		* ���� �� ������ "�����" ������ �������� ���� ���. 
		*/
		Nucleus::Status define_status(const _place&, const vector<_place>&) const;
		/*
		* ��������� ������ �������� ������ �������� ����������� ��������� ��������.
		*/
		vector<_place> mark_neighbors(const _place&) const;
		/*
		* � "������" ������, ����� � ������� ����� ��� "�����", ����������� �����.
		* ����, ����� � "�����" ���� ��� ��� ��� "�����" ������� - ������ ���������� ����.
		* ���� ������� ������ ���� ��� ������ ���, ������ �������.
		*/
		Nucleus::Status live_or_die(const _place&, const size_t);
		/*
		* 1. ��������� ������ �� ������ ��������.
		* 2. �������� ��������� ���������.
		* 3. ������������ ���������� "�����" ������.
		*/
		size_t living_neighbors(const _place&) const;
		/*
		* ������������� �������� �������� �������� ������.
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