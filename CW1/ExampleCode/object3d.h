// Abstract class to represent an object in a scene.

#ifndef OBJECT3D_H_
#define OBJECT3D_H_

#include "part2.h"

class Object3D{

protected:

  // Colour of object.
  Vec3f _color;

public:

  // Constructors:
  Object3D(){};

  Object3D(Vec3f color){ this->_color = color; };

  // Destructor.
  ~Object3D(){};

  // Identify if the given ray intersects the object.  Return true if it does
  // and update the hit object to contain the smallest value of t (ray
  // parameter) and the colour of the point of intersection.
  virtual bool intersect(const Ray &r, Hit &h) = 0;

};

#endif /* OBJECT3D_H_ */
