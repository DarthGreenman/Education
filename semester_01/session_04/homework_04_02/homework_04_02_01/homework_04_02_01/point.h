/* point.h */

#ifndef MY_POINT_H
#define MY_POINT_H

struct point {
	double m_x;
	double m_y;
	point(double x, double y) : m_x{ x }, m_y{ y } {}
};

void print_point(const point& point_object);

#endif /* MY_POINT_H */
