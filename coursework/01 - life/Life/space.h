// space.h

#ifndef SPACE_H
#define SPACE_H

#include <utility>
#include <vector>

namespace space {

	using _place = std::pair<size_t, size_t>;

	bool operator==(const _place&, const _place&);
	bool operator!=(const _place&, const _place&);
	/*
	* ��������� "�����" �����.
	* ������� ����������� ������������� ������������� ��������
	* � ���������.
	*/
	std::vector<_place> make_life(const _place&, size_t);
}
#endif /* SPACE_H */