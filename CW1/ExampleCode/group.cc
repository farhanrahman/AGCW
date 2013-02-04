#include "part2.h"

// Constructor for a group of n objects.
Group::Group(int n) {

  this->_object = new Object3D*[n];
  this->_count  = n;
}

// Destructor.
Group::~Group() {

  if (this->_object == NULL) {
    return;
  }

  for (int i = 0; i < this->_count; i++) {
    delete this->_object[i];
  }

  delete[] this->_object;
}

// Insert an object into the array.
void Group::addObject(int index, Object3D *obj) {
    //Add the obj object to the current list of objects.
    
    (*this)._object[index] = obj;
    
}

bool Group::intersect(const Ray &r, Hit &h)
{
    /*For every object currently in the list, go through them
     *and calculate the intersection point with that object.
     *Update minimum distance with the minimum distance of the
     *intersection of the Ray with an object i.e. an object
     *closest to the screen. If the minimum distance is <
     *than a distance of INFINITY, then return true i.e. there
     *has been a successful intersection with an object*/
    
    float min_distance = INFINITY; //initialise min_distance to INFINITY. Assume no intersection
    //cout << "Grpup intersect called" << endl;
    for (int i = 0; i < _count; i++) { //loop through every object.
        if(_object[i]->intersect(r, h)){ //determine the intersect point of object and ray
            if (h.getT() < min_distance) {
                min_distance = h.getT();//update the minimum distance
            }
        }
    }

    return min_distance != INFINITY;
}
