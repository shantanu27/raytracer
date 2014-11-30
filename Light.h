#ifndef _LIGHT_H
#define _LIGHT_H

#include "Vector.h"
#include "Color.h"

class Light {
	public:
		Light();
		Light (Vector position, Color lightColor);

		//method functions
		Vector getPosition() { return position; }
		Color getColor() { return color; }

	private:
		Vector position;
		Color color;
};

Light::Light () 
{
	position = Vector (0.0, 0.0, 0.0);
	color = Color (1.0, 1.0, 1.0, 0.0);
}

Light::Light (Vector pos, Color lightColor) 
{
	position = pos;
	color = lightColor;
}

#endif