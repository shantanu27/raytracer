#include "Vector.h"

#include <cmath>
#include <iostream>

using namespace std;

Vector::Vector() 
{
	x = 0;
	y = 0;
	z = 0;
}

Vector::Vector(double i, double j, double k) 
{
	x = i;
	y = j;
	z = k;
}

double Vector::magnitude() 
{
	return sqrt((x*x) + (y*y) + (z*z));
}

Vector Vector::scalarDivide (double s) {
	if (s != 0.0) {
		return Vector(x/s, y/s, z/s);
	} else {
		cout<<"ERROR!! Divide by zero error!"<<endl;
	}
}

Vector Vector::normalize () 
{
	double vecMagnitude = sqrt((x*x) + (y*y) + (z*z));

	if (vecMagnitude > 0) {
		return Vector(x/vecMagnitude, y/vecMagnitude, z/vecMagnitude);
	} else {
		cout<<"ERROR!! Vector with magnitude 0 not allowed"<<endl;
		return Vector(0.0,0.0,0.0);
	}
}
 
