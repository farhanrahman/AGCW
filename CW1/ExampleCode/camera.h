// Abstract camera class, subclasses might be orthographic, projective, ..

#ifndef CAMERA_H_
#define CAMERA_H_

#include "part2.h"

class Camera{

public:
  // Based on the camera parameters, generate a ray starting at
  // a given pixel location.  The pixel coordinates should be in
  // the range (0, 0) -> (1, 1) where these points represent the
  // corners of the image.  Implemented in subclasses.
  virtual Ray generateRay(Vec2f point) = 0;

};

#endif /* CAMERA_H_ */
