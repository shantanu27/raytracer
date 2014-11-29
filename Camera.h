#ifndef _CAMERA_H
#define _CAMERA_H

#include "Vector.h"

class Camera {
	public:
		Camera ();
		Camera (Vector, Vector, Vector, Vector);

		//method functions
		Vector getPosition() {	return cameraPosition; }
		Vector getDirection() { return cameraDirection; }
		Vector getRight() { return cameraRight; }
		Vector getDown() { return cameraDown; }

	private:
		Vector cameraPosition, cameraDirection, cameraRight, cameraDown;
};

Camera::Camera () 
{
	cameraPosition = Vector(0.0, 0.0, 0.0);
	cameraDirection = Vector(0.0, 0.0, 1.0);
	cameraRight = Vector(0.0, 0.0, 0.0);
	cameraDown = Vector(0.0, 0.0, 0.0);
}

Camera::Camera (Vector pos, Vector dir, Vector right, Vector down) 
{
	cameraPosition = pos;
	cameraDirection = dir;
	cameraRight = right;
	cameraDown = down;
}

#endif