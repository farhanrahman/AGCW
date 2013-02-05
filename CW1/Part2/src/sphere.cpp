#include "sphere.h"

// Add any code you need here and in the corresponding header
// file.

Sphere::Sphere(): centre(Vec3f(0,0,0)),radius(1)
{
	radiussq = radius*radius;
}

Sphere::Sphere(Vec3f centre, float radius):centre(centre),radius(radius)
{
	radiussq = radius*radius;
}

//Intersect ray with this sphere
//If intersected, return true and modify hit appropriately
bool Sphere::intersect(const Ray &r, Vec3f* intersection)
{
	// d = (l . c) +- sqrt( (l . c)^2 - c^2 + r^2 )
	//d = distance from origin (float)
	//l = direction of ray (vec3)
	//c = sphere centre point (shifted to assume ray originates from origin) (vec3)
	//r = sphere radius (vec3)
	//source = http://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection

	//Shift position of sphere to make it relative to origin
	Vec3f c = centre - r.getOrigin();

	//Get direction of the ray
	Vec3f l = r.getDirection();

	// (l . c)
	float ldotc = l.Dot3(c);

	//discriminant =  (l . c)^2 - c^2 + r^2
	float discr = ldotc*ldotc - c.Dot3(c) + radiussq;

	//If discriminant < 0, no intersections
	if (discr < 0) {
		return false;
	}
	//If discriminant >= 0, get nearest point of intersection
	else {
		float dist = ldotc - sqrt(discr);

		*intersection = r.getOrigin() + (r.getDirection() * dist);
//
//		//Test if point of intersection closer than last calculated point of intersection
//		if (dist < h.getT()) {
//			h.set(dist,_color);
//		}
		return true;
	}
}

Vec3f Sphere::getNormalAt(const Ray &r) {
	Vec3f intersection;
	if (intersect(r,&intersection)) {
//		return Vec3f(1.0f,0.0f,0.0f);
		Vec3f normal = intersection - centre;
		normal.Normalize();
		return normal;
	}
	else {
		return Vec3f(0.0f,0.0f,0.0f);
	}
}

Vec3f Sphere::getCentre(void) const{
	return this->centre;
}
