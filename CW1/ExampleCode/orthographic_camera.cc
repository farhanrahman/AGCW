#include "part2.h"

// Add any code you need here and in the corresponding header
// file.


// Constructor with parameters.
OrthographicCamera::OrthographicCamera(Vec3f centre, Vec3f direction, Vec3f up, float size)
{
    /*Initialise the object fields*/
    (*this).centre = centre;
    (*this).projection = direction.Normalized();
    (*this).up = up.Normalized();
    (*this).size = size;
        
    if(up.Normalized().Dot3(direction.Normalized()) != 0){ //If up vector isn't orthogonal to the projection vector
        Vec3f h; //temp horizontal vector
        Vec3f::Cross3(h, up.Normalized(), direction.Normalized()); //Calculate the horizontal vector which is the cross product of up and projection
        Vec3f::Cross3((*this).up, h.Normalized(), direction.Normalized()); //Recalculate the orthogonal up vector
    }

    (*this).up.Normalize(); //Normalise up
    Vec3f::Cross3(horizontal, (*this).up, (*this).projection); //Recalculate the correct horizontal vector.
    horizontal.Normalize(); //Normalize horizontal vector.
}


Ray OrthographicCamera::generateRay(Vec2f point)
{
    /*Note that the screen starts from (0,0) from top left
     *and goes to the bottom right to (1,1)*/
    
    Vec3f tl(centre - (horizontal*(size/2)) + (up*(size/2))); //Start from top left of screen.
    
    /*Then calculate the origin vector in world co-ordinates*/
    Vec3f _or(tl - up*((point.y())*size) + horizontal*((point.x())*size));
    
    return Ray((*this).projection, _or);
}


