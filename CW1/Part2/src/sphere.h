// Class for Sphere subclass of Object3D

// Add any code you need here and in the corresponding .cc file.

#ifndef SPHERE_H_
#define SPHERE_H_

#include "vectors.h"
#include "ray.h"

class Sphere{
private:
	Vec3f centre;
	float radius;
	float radiussq;
public:

  // Default constructor.
  Sphere();

  // Constructor with parameters specified.
  Sphere(Vec3f centre, float radius);

  // A concrete implementation of pure virtual function in parent
  // class.
  bool intersect(const Ray &r, Vec3f* intersection);

  Vec3f getNormalAt(const Ray &r);

  Vec3f getCentre(void) const;

};

#endif /* SPHERE_H_ */
