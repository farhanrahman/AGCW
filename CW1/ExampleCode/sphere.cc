#include "part2.h"

// Add any code you need here and in the corresponding header
// file.


Sphere::Sphere()
{
    (*this).centre = Vec3f(0.0,0.0,0.0);
    (*this).radius = 1;
    (*this)._color = Vec3f(0.5,0.5,0.5);
}



Sphere::Sphere(Vec3f centre, float radius, Vec3f color)
{
    (*this).centre = centre;
    (*this).radius = radius;
    (*this)._color = color;
}



bool Sphere::intersect(const EnvironmentMap& em, const Ray &r, Hit &h)
{
    Vec3f d      = r.getDirection();
    Vec3f deltaP = ((*this).centre - r.getOrigin());
    
    float ddotdp = d.Dot3(deltaP);
    
    float ddotdpsq = ddotdp*ddotdp;
    
    float determinant = ddotdpsq
                        -(deltaP.Dot3(deltaP)) 
                        + radius*radius;
    if(determinant < 0){ //check if there is an intersection
        /*if there is no intersection, assume that
         *there are no objects behind this obect
         *so don't update the Hit h object*/
        return false;
    }else{
        
        float mu1 = (ddotdp + sqrtf(determinant)); //calculate the mu1 term
        float mu2 = (ddotdp - sqrtf(determinant)); //calculate the mu2 term       
        //Vec3f q = r.pointAtParameter(min(mu1, mu2));
        if(min(mu1,mu2) < h.getT()) { //if the current hit distance is greater than the current intersection point distance 
           	//h.set(min(mu1, mu2), (*this)._color); //update h.

//        h.set(Vec3f(1.0f,1.0f,1.0f));

		Vec3f n = r.pointAtParameter(min(mu1,mu2)) - (*this).centre;
		n.Normalize();
		Vec3f v = r.pointAtParameter(min(mu1,mu2)).Negated();
		v.Normalize();
		float ndotv = n.Dot3(v);
		ndotv *= 2.0;
		Vec3f r(n.Scale(ndotv) - v);
		LatLong ll(r);
		h.set(min(mu1,mu2), em.mapTo(ll));
	}
    }
    
    return true;
}
