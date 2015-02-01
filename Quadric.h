#ifndef _QUADRIC_H
#define _QUADRIC_H

#include <cmath>

#include "Color.h"
#include "Object.h"
#include "Vector.h"

class Quadric : public Object {
	public:
		Quadric();
		Quadric(Vector Q1, Vector Q2, Vector Q3, double k, Color color);

		//method functions
		virtual Color getColor() { return color; }

		virtual Vector normalAt(Vector point) 
		{ 
			double x = point.getX();
			double y = point.getY();
			double z = point.getZ();

			double A = Q1.getX();
			double B = Q1.getY();
			double C = Q1.getZ();

			double D = Q2.getX();
			double E = Q2.getY();
			double F = Q2.getZ();

			double G = Q3.getX();
			double H = Q3.getY();
			double I = Q3.getZ();

			Vector normal = Vector(A*x + B*y + C*z + D,
								   B*x + E*y + F*z + G,
								   C*x + F*y + H*z + I);
			return normal.normalize();
		}

		virtual double findIntersection(Ray ray)
		{
			Vector rayOrigin = ray.getOrigin();
			Vector rayDirection = ray.getDirection();

			double x0 = rayOrigin.getX();
			double y0 = rayOrigin.getY();
			double z0 = rayOrigin.getZ();

			double xd = rayDirection.getX();
			double yd = rayDirection.getY();
			double zd = rayDirection.getZ();

			//Standard quadratic equation in t is : At^2 + B^t + C = 0
			//Follow 3 element vectors will be used to calculate A, B, C
			//Which will be later substituted in the quadratic to calculate t

			Vector V1(xd*xd, yd*yd, zd*zd);
			Vector V2(2*xd*yd, 2*yd*zd, 2*zd*xd);
			Vector V3(2*x0*xd, 2*y0*yd, 2*z0*zd);
			Vector V4(2*(xd*y0 + x0*yd), 2*(y0*zd + yd*z0), 2*(x0*zd + xd*z0));
			Vector V5 = rayDirection.scalarMultiply(2);
			Vector V6(x0*x0, y0*y0, z0*z0);
			Vector V7(2*x0*y0, 2*y0*z0, 2*x0*z0);
			Vector V8 = rayOrigin.scalarMultiply(2);

			//A = Q1 • V1 + Q2 • V2
			double A = Q1.dot(V1) + Q2.dot(V2);

			//B = Q1 • V3 + Q2 • V4 + Q3 • V5
			double B = Q1.dot(V3) + Q2.dot(V4) + Q3.dot(V5);

			//C = Q1 • V6 + Q2 • V7 + Q3 • V8 + k
			double C = Q1.dot(V6) + Q2.dot(V7) + Q3.dot(V8) + size;

			//now solve for t
			double discriminant = B*B - 4*A*C;

			if (discriminant > 0)
			{
				// the ray intersects the quadric

				// the first root 
				double distance1 = ((-1*B - sqrt (discriminant))/2*A) - 0.001;	// to accomodate for float point errors 
																				//(this value may be different for different systems)
				double distance2 = ((sqrt(discriminant) - B)/2*A) - 0.001;		// we make sure the intersection is outside the quadric

				if (distance1 > 0) {
					//whichever is closer
					return distance1; 
				} else {
					// the second root is the smallest positive root
					return distance2;
				}
			} else {
					// the ray missed the quadric
					return -1;
			}
		}

	private:
		// Q1(a,b,c), Q2(d,e,f), Q3(g,h,i)
		// represents general equation of a quadric
		Vector Q1, Q2, Q3;
		double size;
};

Quadric::Quadric()
{	
	//default is a sphere
	Q1 = Vector(1, 1, 1);
	Q2 = Vector(0, 0, 0);
	Q3 = Vector(0, 0, 0);
	size = -1;
	color = Color(1, 1, 1);
}

Quadric::Quadric(Vector p1, Vector p2, Vector p3, double k, Color quadricColor)
{
	Q1 = p1;
	Q2 = p2;
	Q3 = p3;
	size = k;
	color = quadricColor;
}

#endif