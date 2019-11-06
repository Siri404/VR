#include <cmath>
#include <iostream>
#include <string>

#include "Vector.h"
#include "Line.h"
#include "Geometry.h"
#include "Sphere.h"
#include "Image.h"
#include "Color.h"
#include "Intersection.h"
#include "Material.h"

#include "Scene.h"

using namespace std;
using namespace rt;

float imageToViewPlane(int n, int imgSize, float viewPlaneSize) {
	float u = (float)n*viewPlaneSize / (float)imgSize;
	u -= viewPlaneSize / 2;
	return u;
}

const Intersection findFirstIntersection(const Line& ray,
	float minDist, float maxDist) {
	Intersection intersection;

	for (int i = 0; i < geometryCount; i++) {
		Intersection in = scene[i]->getIntersection(ray, minDist, maxDist);
		if (in.valid()) {
			if (!intersection.valid()) {
				intersection = in;
			}
			else if (in.t() < intersection.t()) {
				intersection = in;
			}
		}
	}

	return intersection;
}

int main() {
	Vector viewPoint(-50, 10, 0);
	Vector viewDirection(50, -10, 100);
	Vector viewUp(0, -1, 0);

	viewDirection.normalize();
	viewUp.normalize();

	float frontPlaneDist = 0;
	float backPlaneDist = 1000;

	float viewPlaneDist = 65;
	float viewPlaneWidth = 160;
	float viewPlaneHeight = 120;

	int imageWidth = 1024;
	int imageHeight = 768;

	Vector viewParallel = viewUp ^ viewDirection;
	viewParallel.normalize();

	Image image(imageWidth, imageHeight);

	// ADD CODE HERE
	//Color Black = Color(0, 0, 244);
	Color Black = Color(0, 0, 0);
	//Color Black = Color(0, 255, 0);
	for (int i = 0; i < imageWidth; i++) {
		for (int j = 0; j < imageHeight; j++) {
			Vector v = viewPoint + viewDirection*viewPlaneDist + viewUp*imageToViewPlane(j, imageHeight, viewPlaneHeight)
				+ viewParallel * imageToViewPlane(i, imageWidth, viewPlaneWidth);
			Line ray = Line(viewPoint, v, false);
			Intersection intersection = findFirstIntersection(ray, frontPlaneDist, backPlaneDist);
			if (intersection.valid()) {
				Color color = intersection.geometry()->material().ambient();
				for (int l = 0; l < lightCount; ++l) {
					Sphere* sp = (Sphere*)intersection.geometry();
					Vector N = intersection.vec() - sp->center();
					Vector T = lights[l]->position() - intersection.vec();
					N.normalize();
					T.normalize();

					if (N * T > 0)
					{
						color += intersection.geometry()->material().diffuse() * lights[l]->diffuse() * (N * T);
					}

					Vector E = viewPoint - intersection.vec();
					Vector R = N * (float)(2.0 * (N * T)) - T;
					E.normalize();
					R.normalize();

					if (E * R > 0)
					{
						color += lights[l]->specular() * intersection.geometry()->material().specular() * (pow(E * R, intersection.geometry()->material().shininess()));
					}
					color *= lights[l]->intensity();
				}
				image.setPixel(i, j, color);
			}
			else {
				image.setPixel(i, j, Black);
			}
		}
	}
	image.store("scene.png");

	for (int i = 0; i < geometryCount; i++) {
		delete scene[i];
	}

	for (int i = 0; i < lightCount; i++) {
		delete lights[i];
	}

	return 0;
}
