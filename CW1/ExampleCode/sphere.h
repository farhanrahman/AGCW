// Class for Sphere subclass of Object3D

// Add any code you need here and in the corresponding .cc file.

#ifndef SPHERE_H_
#define SPHERE_H_

#include "part2.h"

class Sphere : public Object3D{

private:
    Vec3f centre;
    float radius;
    int ID;

public:

  // Default constructor.
  Sphere();

  // Constructor with parameters specified.
  Sphere(Vec3f centre, float radius, Vec3f color);

  // A concrete implementation of pure virtual function in parent
  // class.
  virtual bool intersect(const Ray &r, Hit &h);

};

#endif /* SPHERE_H_ */
