#ifndef _OBJECT_H
#define _OBJECT_H

#include "Color.h"
#include "Ray.h"
#include "Vector.h"

class Object {
	public:
		Object();

		//method functions
		double getDiffuseCoefficient() { return diffuseCoefficient; }
		double getSpecularCoefficient() { return specularCoefficient; }
		double getReflectivity() { return reflectivity; }
		double getPhongShininess() { return phongShininess; }

		void setDiffuseCoefficient(double kd) { diffuseCoefficient = kd; }
		void setSpecularCoefficient(double ks) { specularCoefficient = ks; }
		void setReflectivity(double pr) { reflectivity = pr; }
		void setPhongShininess(double n) { phongShininess = n; }

		virtual Color getColor() { return Color(0.0, 0.0, 0.0); }
		virtual Vector normalAt(Vector point) { return Vector(0,0,0); }

		virtual double findIntersection(Ray ray) { return 0; }

	protected:
		Color color;
		double diffuseCoefficient;
		double specularCoefficient;
		double reflectivity;
		double phongShininess;
};

Object::Object () 
{
	diffuseCoefficient = 0;
	specularCoefficient = 0;
	reflectivity = 0;
	phongShininess = 0;
}

#endif