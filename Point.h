#ifndef _POINT_H
#define _POINT_H

#include "Vector.h"

class Point {
	public:
		Point();
		Point(double x, double y, double z);

		double x() { return x; }
		double y() { return y; }
		double z() { return z; }

		Point add (Vector v) { return Point(x + v.x(), y + v.y(), z + v.z()); }
		Point subtract (Vector v) { return Point(x - v.x(), y - v.y(), z - v.z()); }

	private:
		double x;
		double y;
		double z;
};   

#endif