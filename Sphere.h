#ifndef _SPHERE_H
#define _SPHERE_H

#include <cfloat>
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

		virtual Vector normalAt(Vector point) 
		{
			//normal points away from the center
			Vector normalVector = (point + center.negative()).normalize();
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
				double distance1 = (((-1*b - sqrt (discriminant))/2) - 0.001);	// to accomodate for float point errors 
																				//(this value may be different for different systems)
				double distance2 = ((sqrt(discriminant) - b)/2) - 0.001;		// we make sure the intersection is outside the sphere

				if (distance1 > 0) {
					//whichever is closer
					return distance1; 
				} else {
					// the second root is the smallest positive root
					return distance2;
				}
			} else {
					// the ray missed the sphere
					return -1;
			}
		}

	protected:
		Vector center;
		double radius;
};

Sphere::Sphere () 
{
	center = Vector (0.0, 0.0 , 0.0);
	radius = 1.0;
	color = Color (0.5, 0.5, 0.5);
}

Sphere::Sphere (Vector centerValue, double radiusValue, Color sphereColor)
	:Object() 
{
	center = centerValue;
	radius = radiusValue;
	color = sphereColor;
}

#endif