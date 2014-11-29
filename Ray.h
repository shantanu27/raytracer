#ifndef _RAY_H
#define _RAY_H

#include "Vector.h"

class Ray {
	public:
		Ray();
		Ray (Vector origin, Vector direction);

		//method functions
		Vector getOrigin() { return origin; }

		Vector getDirection() { return direction; }
	private:
		Vector origin;
		Vector direction;
};

Ray::Ray () 
{
	origin = Vector (0.0, 0.0, 0.0);
	direction = Vector (1.0, 0.0, 0.0);
}

Ray::Ray (Vector o, Vector d) 
{
	origin = o;
	direction = d;
}

#endif