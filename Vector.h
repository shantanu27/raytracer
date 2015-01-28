#ifndef _VECT_H
#define _VECT_H

class Vector {
	public:
		Vector();
		Vector(double, double, double);

		double getX() const { return x; }
		double getY() const { return y; }
		double getZ() const { return z; }

		double magnitude();

		Vector negative () { return Vector(-x, -y, -z); }

		Vector add (Vector v) 
		{
			return Vector(x + v.getX(), y + v.getY(), z + v.getZ());
		}

		Vector subtract (Vector v) 
		{
			return Vector(x - v.getX(), y - v.getY(), z - v.getZ());
		}

		Vector scalarMultiply (double s) 
		{
			return Vector(x*s, y*s, z*s);
		}

		Vector scalarDivide (double s);
		Vector normalize ();

		double dotProduct (Vector v) 
		{
			return x*v.getX() + y*v.getY() + z*v.getZ();
		}

		Vector crossProduct (Vector v) 
		{
			return Vector(y*v.getZ() - z*v.getY(), z*v.getX() - x*v.getZ(), x*v.getY() - y*v.getX());
		}

		Vector operator+ (const Vector &v)
		{
			return Vector(x + v.getX(), y + v.getY(), z + v.getZ());
		}

		Vector operator- (const Vector &v)
		{
			return Vector(x - v.getX(), y - v.getY(), z - v.getZ());
		}

	private:
		double x;
		double y;
		double z;
};

#endif
