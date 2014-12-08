#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cfloat>
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

#define MAX_RAY_DEPTH 5

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

Color computeColorAtIntersection(Vector intersectPosition, 
								Vector intersectDirection, 
								vector<Object*> sceneObjects, 
								int indexOfClosestObject, 
								double lightAmbience,
								vector<Light>& lightSources,
								int depth)
{
	Object* currentObject = sceneObjects.at(indexOfClosestObject);
	Color surfaceColor = currentObject->getColor();
	Vector surfaceNormal = currentObject->normalAt(intersectPosition);
	bool isObjectInShadow = false;

	//get the properties of sphere to be used later for specular, diffuse and ambient lighting
	double kd = currentObject->getDiffuseCoefficient();
	double ks = currentObject->getSpecularCoefficient();
	double kr = currentObject->getReflectivity();
	double n = currentObject->getPhongShininess();

	Color finalColor(0,0,0);
	Color diffuse(0,0,0);
	Color specular(0,0,0);
	Color ambient(0,0,0);

	/* Reflected ray would be used for calculating specular component */
	/* and the reflectivity */

	if (ks == 2) {
		//checkerboard pattern
		int square = (int)floor(intersectPosition.getX()) + (int)floor(intersectPosition.getZ());
		if (square % 2 == 0) {
			//black square
			surfaceColor.setRed(0);
			surfaceColor.setGreen(0);
			surfaceColor.setBlue(0);
		} else {
			//white square
			surfaceColor.setRed(1);
			surfaceColor.setGreen(1);
			surfaceColor.setBlue(1);
		}
	}

	/*Add reflectivity*/
	if ((kr > 0 && kr <= 1) && (depth< MAX_RAY_DEPTH)) {
		//calculate reflected ray
		//R = I - 2(i.n)N - uppercase are vectors
		Vector I = intersectDirection;
		double IDotN = I.dotProduct(surfaceNormal);
		Vector IDotNVector = surfaceNormal.scalarMultiply(IDotN);
		IDotNVector = IDotNVector.scalarMultiply(2);
		Vector reflectedRayDirection = I.subtract(IDotNVector);		//already normalized
		reflectedRayDirection = reflectedRayDirection.normalize();

		Ray reflectedRay(intersectPosition, reflectedRayDirection);

		vector<double> reflectedRayIntersections;
		for (vector<Object*>::iterator object = sceneObjects.begin(); object != sceneObjects.end(); ++object) {
			reflectedRayIntersections.push_back((*object)->findIntersection(reflectedRay));
		}

		int indexOfclosestReflectedObject = closestObjectIndex(reflectedRayIntersections);
		if (indexOfclosestReflectedObject != -1) {
			//reflected ray hit some object in the scene
			//the color at this point on the object would have some component
			//of the object which the reflected ray hit
			if (reflectedRayIntersections.at(indexOfclosestReflectedObject) > FLT_EPSILON) {
				Vector reflectedObjectIntersectionPosition = intersectPosition.add(reflectedRayDirection.scalarMultiply(reflectedRayIntersections.at(indexOfclosestReflectedObject)));
				Vector reflectedObjectDirection = reflectedRayDirection;
				Color reflectedColor = computeColorAtIntersection(reflectedObjectIntersectionPosition,
																  reflectedObjectDirection,
																  sceneObjects,
																  indexOfclosestReflectedObject,
																  lightAmbience,
																  lightSources,
																  depth + 1);

				//add the color based on the objects reflectivity
				finalColor = finalColor.add(reflectedColor.scalarMultiply(kr));
			}
		}
	}

	for (vector<Light>::iterator light = lightSources.begin(); light != lightSources.end(); ++light) {

		Color lightColor = light->getColor();

		// rayToLight = lightSourceCenter - intersectPoint
		Vector vectorToLight = (light->getPosition()).subtract(intersectPosition);
		double lightSourceDistance = vectorToLight.magnitude();
		vectorToLight = vectorToLight.normalize();		

		//ray from intersecting point to lightSource
		Ray rayToLight(intersectPosition, vectorToLight);

		vector<double> secondaryIntersections;

		//find if the ray intersected any of the objects in the scene
		for (vector<Object*>::iterator object = sceneObjects.begin(); object != sceneObjects.end(); ++object) {	
			secondaryIntersections.push_back((*object)->findIntersection(rayToLight));
		}

		// find closest object from the intersections
		for (vector<double>::iterator distance = secondaryIntersections.begin(); distance != secondaryIntersections.end(); ++distance) {
			if ((*distance) > FLT_EPSILON) {
				if (*distance <= lightSourceDistance) {
					isObjectInShadow = true;
					break;
				}
			}
		}

		//add ambient component
		if (kd > 0) {
			ambient = lightColor.multiply(surfaceColor.scalarMultiply(lightAmbience*kd));
		} else {
			ambient = lightColor.multiply(surfaceColor.scalarMultiply(lightAmbience));
		}	

		finalColor = finalColor.add(ambient);

		if (isObjectInShadow == false) {

			//if lightDotNormal is < 0 means we are behind the object (its in shadow)
			double lightDotNormal = surfaceNormal.dotProduct(vectorToLight);
			lightDotNormal = lightDotNormal < 0 ? 0.0f : lightDotNormal;

			//add diffuse component
			if (kd > 0) {
				diffuse = diffuse.add(((surfaceColor.scalarMultiply(kd)).multiply(lightColor)).scalarMultiply(lightDotNormal));
			}

			Vector L = vectorToLight;	//vector from intersection point to light normalized
			Vector N = surfaceNormal;	// surface normal
			Vector V = intersectDirection.negative().normalize();	//vector towards eye point

			double LDotN = L.dotProduct(N);
			Vector R = (N.scalarMultiply(2*LDotN)).subtract(L);		//reflected ray

			double RDotV = R.dotProduct(V);
			
			//add specular component
			if (ks > 0 && ks <=1 && n > 0) {
				specular = lightColor.scalarMultiply(ks*pow(RDotV,n)); 
			}
			
			//Total illumination = ambient + diffuse + specular (ambient has already been added)
			finalColor = (finalColor.add(diffuse)).add(specular);			
		} 
	}

	//default color is black
	return finalColor.clip();
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

	//camera specs
	Vector cameraPos(3.0, 1.5, -4.0); 
	Vector lookAt(0.0, 0.0, 0.0);

	Vector viewDirection(cameraPos.getX()-lookAt.getX(), 
					   cameraPos.getY()-lookAt.getY(), 
					   cameraPos.getZ()-lookAt.getZ());

	Vector cameraDirection = viewDirection.negative().normalize();
	Vector cameraRight = Y.crossProduct(cameraDirection).normalize();		//for cameraUp ==> cameraDirection x Up
	Vector cameraDown = cameraRight.crossProduct(cameraDirection).normalize();

	Camera sceneCamera(cameraPos, cameraDirection, cameraRight, cameraDown);

	Color white(1.0, 1.0, 1.0);
	Color prettyGreen(0.5, 1.0, 0.5);
	Color maroon(0.5, 0.25, 0.25);
	Color gray(0.5, 0.5, 0.5);
	Color black(0.0, 0.0, 0.0);
	Color red(1, 0.2, 0);

	//define light ambience and store light sources in a vector
	double lightAmbience = 0.2;
	vector<Light> lightSources;

	Vector lightPosition1(-7.0, 10.0, -10.0);
	Light sceneLight1(lightPosition1, white);
	lightSources.push_back(sceneLight1);

	//Vector to store the current objects in the scene
	vector<Object *> sceneObjects;

	//scene description
	Sphere sceneSphere1(O, 1, prettyGreen);
	sceneSphere1.setDiffuseCoefficient(0.5);
	sceneSphere1.setSpecularCoefficient(0.3);
	sceneSphere1.setPhongShininess(10);
	sceneSphere1.setReflectivity(0.4);

	Sphere sceneSphere2(Vector(2, 0, 0.5), 0.5, maroon);
	sceneSphere2.setDiffuseCoefficient(0.5);
	sceneSphere2.setSpecularCoefficient(0.3);
	sceneSphere2.setPhongShininess(10);
	sceneSphere2.setReflectivity(0.4);


	Plane scenePlane(Y, -1, white);
	scenePlane.setDiffuseCoefficient(0.8);
	scenePlane.setSpecularCoefficient(0.3);
	scenePlane.setPhongShininess(10);
	//although not a specular coefficient, I will use it to define checkerboard pattern 
	//TODO: figure out a way to map patter
	scenePlane.setSpecularCoefficient(2);

	sceneObjects.push_back(dynamic_cast<Object *>(&sceneSphere1));
	sceneObjects.push_back(dynamic_cast<Object *>(&sceneSphere2));
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

			// return color
			if (indexOfClosestObject == -1) {
				pixels[currentPixel].r = 0;
				pixels[currentPixel].g = 0;
				pixels[currentPixel].b = 0;
			} else {
				// to accomodate for precision errors
				if (intersections.at(indexOfClosestObject) > FLT_EPSILON) {
					//calculate intersection position
					Vector intersectPosition = cameraRayOrigin.add(cameraRayDirection.scalarMultiply(intersections.at(indexOfClosestObject)));
					Vector intersectDirection = cameraRayDirection;

					int depth = 0;
					Color intersectionColor = computeColorAtIntersection(intersectPosition, 
																		intersectDirection, 
																		sceneObjects, 
																		indexOfClosestObject, 
																		lightAmbience,
																		lightSources,
																		depth);

					pixels[currentPixel].r = intersectionColor.getRed();
					pixels[currentPixel].g = intersectionColor.getGreen();
					pixels[currentPixel].b = intersectionColor.getBlue();
				}
			}
		}	
	}

	saveBmp("scene.bmp", width, height, dpi, pixels);

	delete pixels;

	return 0;
}