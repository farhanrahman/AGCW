#ifndef _HIT_H
#define _HIT_H

#include "part2.h"
//class Material

// ====================================================================
// ====================================================================

class Hit {

public:

  // CONSTRUCTOR & DESTRUCTOR
  Hit(float _t, Vec3f c) { t = _t; color = c; }
  ~Hit() {}

  // ACCESSORS
  float getT() const { return t; }
  Vec3f getColor() const { return color; }

  // MODIFIER
  void set(float _t, Vec3f c) { t = _t; color = c; }

private:

  // REPRESENTATION
  float t;
  Vec3f color;
  //Material *material;
  //Vec3f normal;

};

// ====================================================================
// ====================================================================

#endif
