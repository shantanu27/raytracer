#ifndef _PLANE_H
#define _PLANE_H

#include "Color.h"
#include "Plane.h"
#include "Vector.h"

class Plane : public Object {
	public:
		Plane();
		Plane (Vector normal, double distanceValue, Color color);

		//method functions
		Vector getNormal() { return normal; }
		double getDistance() { return distance; }
		virtual Color getColor() { return color; }

		virtual Vector normalAt(Vector point) { return normal; }

		virtual double findIntersection(Ray ray) 
		{
			Vector rayDirection = ray.getDirection();

			double a = rayDirection.dotProduct(normal);

			if (a == 0.0) {
				//ray is parallel to the plane
				return -1;
			} else {
				double b = normal.dotProduct(ray.getOrigin().add(normal.scalarMultiply(distance).negative()));
				return -1*b/a;
			}
		}

	protected:
		Vector normal;
		double distance;
};

Plane::Plane () 
{
	normal = Vector (1.0 , 0.0, 0.0);
	distance = 0.0;
	color = Color (0.5, 0.5, 0.5);
}

Plane::Plane (Vector normalValue, double distanceValue, Color planeColor)
	:Object() 
{
	normal = normalValue;
	distance = distanceValue;
	color = planeColor;
}

#endif