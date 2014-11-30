#ifndef _COLOR_H
#define _COLOR_H

class Color {
	public:
		Color ();
		Color (double red, double green, double blue);

		double getRed() { return red; }
		double getGreen() { return green; }
		double getBlue() { return blue; }

		double setRed(double redValue) { red = redValue; }
		double setGreen(double greenValue) { green = greenValue; }
		double setBlue(double blueValue) { blue = blueValue; }

		double brightness() { return 0.299*red + 0.587*green + 0.114*blue; }

		Color add(Color c) 
		{
			return Color(red + c.getRed(), green + c.getGreen(), blue + c.getBlue()); 
		}

		Color subtract(Color c) 
		{
			return Color(red - c.getRed(), green - c.getGreen(), blue - c.getBlue()); 
		}

		Color multiply(Color c)
		{
			return Color(red*c.getRed(), green*c.getGreen(), blue*c.getBlue());
		}

		Color scalarMultiply(double s)
		{
			return Color(red*s, green*s, blue*s);
		}

		Color scalarDivide(double s)
		{
			return Color(red/s, green/s, blue/s);
		}

		Color clip() 
		{
			double totalColor = red + green + blue;
			double excessLight = totalColor - 3;
			if (excessLight > 0) {
				red = red + excessLight*(red/totalColor);
				green = green + excessLight*(green/totalColor);
				blue = blue + excessLight*(blue/totalColor);
			}

			if (red > 1) { red = 1; }
			if (green > 1) { green = 1; }
			if (blue > 1) { blue = 1; }
			if (red < 0) { red = 0; }
			if (green < 0) { red = 0; }
			if (blue < 0) { red = 0; }

			return Color(red, green, blue);
		}

	private:
		double red, green, blue;
};

Color::Color () 
{
	red = 0.5;
	green = 0.5;
	blue = 0.5;
}

Color::Color (double r, double g, double b) 
{
	red = r;
	green = g;
	blue = b;
}

#endif