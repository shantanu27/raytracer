#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include <cfloat>

#include "Color.h"
#include "Vector.h"

using namespace std;

class Triangle : public Object {
	public:
		Triangle();
		Triangle (Vector v1, Vector v2, Vector v3, Color color);

		//method functions
		virtual Color getColor() { return color; }

		double getDistance() 
		{ 
			normal = normalAt(v1);
			distance = normal.dot(v1);
			return distance; 
		}

		virtual Vector normalAt(Vector point) 
		{
			//assuming vertices are ordered in counter clockwise direction
			Vector v3v1 = v3 - v1;
			Vector v2v1 = v2 - v1;
			normal = v2v1.cross(v3v1);
			return normal; 
		}

		virtual double findIntersection(Ray ray) 
		{
			Vector rayOrigin = ray.getOrigin();
			Vector rayDirection = ray.getDirection();

			//Moller Trumbore intersection algorithm
			Vector e1 = v2 - v1;
			Vector e2 = v3 - v1;

			//Begin calculating determinant - also used to calculate u parameter
			Vector P = rayDirection.cross(e2);

			//if determinant is near zero, ray lies in plane of triangle
			double determinant = e1.dot(P);

			if (determinant > -FLT_EPSILON && determinant < FLT_EPSILON) {
				return -1;
			}

			double inverseDeterminant = 1.f/determinant;

			//calculate distance from v1 to ray origin
			Vector T = rayOrigin - v1;
			double u = (T.dot(P)) * inverseDeterminant;

			//The intersection lies outside of the triangle
  			if(u < 0.f || u > 1.f) {
  				return -1;
  			}

  			//Prepare to test v parameter
  			Vector Q = T.cross(e1);
  			double v = (rayDirection.dot(Q)) * inverseDeterminant;

  			//The intersection lies outside of the triangle
  			if(v < 0.f || u + v  > 1.f) {
  				return 0;
  			}

  			double t = (e2.dot(Q) * inverseDeterminant);

  			if (t > FLT_EPSILON) {
  				return t;
  			}

  			return -1;
		}

	protected:
		Vector v1, v2, v3;
		Vector normal;
		double distance;
};

Triangle::Triangle () 
{
	v1 = Vector(1, 0 ,0);
	v2 = Vector(0, 1 ,0);
	v3 = Vector(0, 0 ,1);
	color = Color (0.5, 0.5, 0.5);
}

Triangle::Triangle (Vector A, Vector B, Vector C, Color triangleColor)
	:Object() 
{
	v1 = A;
	v2 = B;
	v3 = C;
	color = triangleColor;
}

#endif