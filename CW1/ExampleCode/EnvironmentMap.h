#ifndef ENVIRONMENTMAP_H
#define ENVIRONMENTMAP_H

#include "part2.h"

class EnvironmentMap{
public:
	EnvironmentMap(Image image) : image(image) {}
	~EnvironmentMap() {}

	Vec3f mapTo(float theta, float phi) const{
		unsigned int i = (unsigned int) (theta * ((float) image.height));
		unsigned int j = (unsigned int) (phi * ((float) image.width));
		unsigned int index = (i*image.width + j) * image.numComponents;
		if (index > image.height * image.width * image.numComponents) {
			return Vec3f(1.0f,1.0f,1.0f);
		}
		return Vec3f(image[index], image[index+1], image[index+2]);	
	}

	Vec3f mapTo(const LatLong& ll) const{
		float theta = ll.theta();
		float phi = ll.phi();
		return this->mapTo(theta, phi);	
	}

private:
	Image image;
};

#endif
