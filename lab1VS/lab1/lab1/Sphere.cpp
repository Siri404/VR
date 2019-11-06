#include "Sphere.h"

using namespace rt;

Intersection Sphere::getIntersection(const Line& line, float minDist, float maxDist) {
	Intersection in;

	//X = at+b Y = ct+d Z = et+f parametric equation of the line

	float R = this->_radius;

	float A = line.dx()*line.dx();
	float B = line.dx()*line.x0() * 2 - line.dx()*this->center()*2;
	float C = line.x0()*line.x0() + this->center()*this->center() - line.x0()*this->center() * 2;

	float delta = B * B - 4 * A*(C - R * R);
	if (delta < 0)
	{
		in = Intersection();
	}
	else if (delta > 0)
	{
		float t1 = (-1)*(B + sqrt(delta)) / (2 * A);
		float t2 = (-1)*(B - sqrt(delta)) / (2 * A);
		float t;
		if (t1 < t2) t = t1; else t = t2;
		if (t > 0) {
			in = Intersection(true, (Geometry *)this, &line, t);
		} 
		else in = Intersection();
	}
	else {
		float t = (-1) * B / (2 * A);
		if (t > 0) {
			in = Intersection(true, (Geometry *)this, &line, t);
		} 
		 else in = Intersection();
	}

	return in;
}


const Vector Sphere::normal(const Vector& vec) const {
    Vector n = vec - _center;
    n.normalize();
    return n;
}