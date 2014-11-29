#ifndef _COLOR_H
#define _COLOR_H

class Color {
	public:
		Color ();
		Color (double red, double green, double blue, double special);

		double getRed() { return red; }
		double getGreen() { return green; }
		double getBlue() { return blue; }
		double getSpecial() { return special; }

		double setRed(double redValue) { red = redValue; }
		double setGreen(double greenValue) { green = greenValue; }
		double setBlue(double blueValue) { blue = blueValue; }
		double setSpecial(double specialValue) { special = specialValue; }

	private:
		double red, green, blue, special;
};

Color::Color () 
{
	red = 0.5;
	green = 0.5;
	blue = 0.5;
}

Color::Color (double r, double g, double b, double s) 
{
	red = r;
	green = g;
	blue = b;
	special = s;
}

#endif