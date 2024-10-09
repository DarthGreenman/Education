/* point.cpp */

#include "point.h"
#include <iostream>

void print_point(const point& point_object) {
	std::cout << "x: " << point_object.m_x << ", y: "
		<< point_object.m_y << std::endl;
}