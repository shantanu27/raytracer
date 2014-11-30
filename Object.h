#ifndef _OBJECT_H
#define _OBJECT_H

#include "Color.h"
#include "Ray.h"
#include "Vector.h"

class Object {
	public:
		Object();

		//method functions
		virtual Color getColor() { return Color(0.0, 0.0, 0.0, 0.0); }


		virtual double findIntersection(Ray ray) { return 0; }

	protected:
		Color color;
		double diffuseCoefficient;
		double specularCoefficient;
		double reflectivity;
		double phongShininess;
};

Object::Object () {}

#endif