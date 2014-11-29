#ifndef _SPHERE_H
#define _SPHERE_H

#include <cmath>

#include "Color.h"
#include "Sphere.h"
#include "Vector.h"

class Sphere : public Object {
	public:
		Sphere();
		Sphere (Vector center, double radius, Color color);

		//method functions
		Vector getCenter() { return center; }
		double getRadius() { return radius; }
		virtual Color getColor() { return color; }

		Vector normalAt(Vector point) 
		{
			//normal points away from the center
			Vector normalVector = point.add(center.negative()).normalize();
			return normalVector;
		}

		virtual double findIntersection(Ray ray) 
		{
			Vector rayOrigin = ray.getOrigin();
			double rayOriginX = rayOrigin.getX();
			double rayOriginY = rayOrigin.getY();
			double rayOriginZ = rayOrigin.getZ();

			Vector rayDirection = ray.getDirection();
			double rayDirectionX = rayDirection.getX();
			double rayDirectionY = rayDirection.getY();
			double rayDirectionZ = rayDirection.getZ();

			Vector sphereCenter = center;
			double sphereCenterX = sphereCenter.getX();
			double sphereCenterY = sphereCenter.getY();
			double sphereCenterZ = sphereCenter.getZ();

			double a = 1; // normalized
			double b = (2*(rayOriginX - sphereCenterX)*rayDirectionX) + (2*(rayOriginY - sphereCenterY)*rayDirectionY) + (2*(rayOriginZ - sphereCenterZ)*rayDirectionZ);
			double c = pow(rayOriginX - sphereCenterX, 2) + pow(rayOriginY - sphereCenterY, 2) + pow(rayOriginZ - sphereCenterZ, 2) - (radius*radius);

			double discriminant = b*b - 4*c;

			if (discriminant > 0)
			{
				// the ray intersects the sphere

				// the first root 
				double root_1 = (((-1*b - sqrt (discriminant))/2) - 0.000001);

				if (root_1 >0) {
					// the first root is the smallest positive root
					return root_1; 
				} else {
					// the second root is the smallest positive root
					double root_2 = ((sqrt(discriminant) - b)/2) - 0.000001;
					return root_2;
				}
			} else {
					// the ray missed the sphere
					return -1;
			}
		}

	private:
		Vector center;
		double radius;
		Color color;
};

Sphere::Sphere () 
{
	center = Vector (0.0, 0.0 , 0.0);
	radius = 1.0;
	color = Color (0.5, 0.5, 0.5, 0.0);
}

Sphere::Sphere (Vector centerValue, double radiusValue, Color sphereColor) 
{
	center = centerValue;
	radius = radiusValue;
	color = sphereColor;
}

#endif