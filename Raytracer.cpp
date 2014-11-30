#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <limits>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Object.h"
#include "Plane.h"
#include "Ray.h"
#include "Sphere.h"
#include "Vector.h"

using namespace std;

struct RGBType {
	double r;
	double g;
	double b;
};

void saveBmp (const char *filename, int w, int h, int dpi, RGBType *data)
{
	FILE *f;
	int k = w*h;
	int s = 4*k;
	int filesize = 54 +s;

	double factor = 39.375;
	int m = static_cast<int>(factor);

	int ppm = dpi * m;

	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};

	bmpfileheader[ 2] = (unsigned char) (filesize);
	bmpfileheader[ 3] = (unsigned char) (filesize>>8);
	bmpfileheader[ 4] = (unsigned char) (filesize>>16);
	bmpfileheader[ 5] = (unsigned char) (filesize>>24);

	bmpinfoheader[ 4] = (unsigned char) (w);
	bmpinfoheader[ 5] = (unsigned char) (w>>8);
	bmpinfoheader[ 6] = (unsigned char) (w>>16);
	bmpinfoheader[ 7] = (unsigned char) (w>>24);

	bmpinfoheader[ 8] = (unsigned char) (h);
	bmpinfoheader[ 9] = (unsigned char) (h>>8);
	bmpinfoheader[ 10] = (unsigned char) (h>>16);
	bmpinfoheader[ 11] = (unsigned char) (h>>24);

	bmpinfoheader[ 21] = (unsigned char) (s);
	bmpinfoheader[ 22] = (unsigned char) (s>>8);
	bmpinfoheader[ 23] = (unsigned char) (s>>16);
	bmpinfoheader[ 24] = (unsigned char) (s>>24);

	bmpinfoheader[ 25] = (unsigned char) (ppm);
	bmpinfoheader[ 26] = (unsigned char) (ppm>>8);
	bmpinfoheader[ 27] = (unsigned char) (ppm>>16);
	bmpinfoheader[ 28] = (unsigned char) (ppm>>24);

	bmpinfoheader[ 29] = (unsigned char) (ppm);
	bmpinfoheader[ 30] = (unsigned char) (ppm>>8);
	bmpinfoheader[ 31] = (unsigned char) (ppm>>16);
	bmpinfoheader[ 32] = (unsigned char) (ppm>>24);

	f = fopen(filename, "wb");

	fwrite(bmpfileheader,1,14,f);
	fwrite(bmpinfoheader,1,40,f);

	for(int i = 0; i < k; i++) {
		RGBType rgb = data[i];

		double red = (data[i].r)*255;
		double green = (data[i].g)*255;
		double blue = (data[i].b)*255;

		unsigned char color[3] = {(int)floor(blue),(int)floor(green),(int)floor(red)};

		fwrite(color,1,3,f);
	}

	fclose(f);
}

int closestObjectIndex(vector<double> objects) 
{
	// returns the index of the winning intersection 
	int minimumValueIndex;

	// prevent unnecessary calculations
	if (objects.size() == 0) {
		// if there are no intersections
		return -1;
	} else if (objects.size() == 1) {
		if (objects.at(0) > 0) {
			// if that intersection is greater than zero then its minimum value is zero
			return 0;
		} else {
			// otherwise the only intersection value is negative
			return -1;
		}
	} else {
		// otherwise there is more than one intersection 
		// first find the maximum value

		double max = 0;
		for (int i = 0; i < objects.size(); i++)
		{
			if (max < objects.at(i))
			{
				max = objects.at(i);
			}
		}

		// then starting from the maximum value find the minimum positive value
		if(max > 0)
		{
			// we only want positive intersections
			for (int index = 0; index < objects.size(); index++)
			{
				if (objects.at(index) > 0 && objects.at(index) <= max)
				{
					max = objects.at(index);
					minimumValueIndex = index;
				}
			}

			return minimumValueIndex;
		} else {
			// all the intersections were negative
			return -1;
		}

	}
}

int currentPixel;

int main (int argc, char* argv[]) 
{

	cout<<"Rendering..."<<endl;

	int dpi = 72;
	int width = 640;
	int height = 480;
	int n = width*height;

	double  aspectRatio = (double)width / (double)height;
	double xAmnt, yAmnt;

	RGBType *pixels = new RGBType[n];

	Vector O(0.0, 0.0, 0.0);
	Vector X(1.0, 0.0, 0.0);
	Vector Y(0.0, 1.0, 0.0);	//Up vector
	Vector Z(0.0, 0.0, 1.0);

	Vector cameraPos(3.0, 1.5, -4.0); 
	Vector lookAt(0.0, 0.0, 0.0);

	Vector viewDirection(cameraPos.getX()-lookAt.getX(), 
					   cameraPos.getY()-lookAt.getY(), 
					   cameraPos.getZ()-lookAt.getZ());

	Vector cameraDirection = viewDirection.negative().normalize();
	Vector cameraRight = Y.crossProduct(cameraDirection).normalize();		//for cameraUp ==> cameraDirection x Up
	Vector cameraDown = cameraRight.crossProduct(cameraDirection).normalize();

	Camera sceneCamera(cameraPos, cameraDirection, cameraRight, cameraDown);

	Color white(1.0, 1.0, 1.0, 0.0);
	Color prettyGreen(0.5, 1.0, 0.5, 0.3);
	Color maroon(0.5, 0.25, 0.25, 0.0);
	Color gray(0.5, 0.5, 0.5, 0.0);
	Color black(0.0, 0.0, 0.0, 0.0);

	Vector lightPosition(-7.0, 10.0, -10.0);
	Light sceneLight(lightPosition, white);

	//Vector to store the current objects in the scene
	vector<Object *> sceneObjects;

	//scene description
	Sphere sceneSphere(O, 1, prettyGreen);
	Plane scenePlane(Y, -1, maroon);

	sceneObjects.push_back(dynamic_cast<Object *>(&sceneSphere));
	sceneObjects.push_back(dynamic_cast<Object *>(&scenePlane));
		
	for (int x=0; x < width; x++) {
		for (int y=0; y < height; y++ ) {
			currentPixel = y*width + x;

			// no anti aliasing
			if (width  > height) {
				// the image is wider than it is tall
				xAmnt = ((x+0.5)/width)*aspectRatio - (((width - height)/(double)height)/2);
				yAmnt = ((height - y) + 0.5)/height;
			} else if (height > width) {
				// the image is taller than it is wide
				xAmnt = (x + 0.5)/width;
				yAmnt = (((height - y) + 0.5)/height)/aspectRatio - (((height - width)/(double)width)/2);
			} else {
				// the image is square
				xAmnt = (x + 0.5)/width;
				yAmnt = ((height - y) + 0.5)/height;
			}

			Vector cameraRayOrigin = sceneCamera.getPosition();
			Vector cameraRayDirection = cameraDirection.add(cameraRight.scalarMultiply(xAmnt - 0.5).add(cameraDown.scalarMultiply(yAmnt - 0.5))).normalize();
			Ray cameraRay(cameraRayOrigin, cameraRayDirection);

			vector<double> intersections;

			for (int index = 0; index < sceneObjects.size(); index++) {
				intersections.push_back(sceneObjects.at(index)->findIntersection(cameraRay));
			}

			int indexOfClosestObject = closestObjectIndex(intersections);
			cout<<indexOfClosestObject;

			// return color
			if (indexOfClosestObject == -1) {
				pixels[currentPixel].r = 0;
				pixels[currentPixel].g = 0;
				pixels[currentPixel].b = 0;
			} else {
				Color pixelColor = sceneObjects.at(indexOfClosestObject)->getColor();
				pixels[currentPixel].r = pixelColor.getRed();
				pixels[currentPixel].g = pixelColor.getGreen();
				pixels[currentPixel].b = pixelColor.getBlue();
			}
		}
	}

	saveBmp("scene.bmp", width, height, dpi, pixels);

	return 0;
}