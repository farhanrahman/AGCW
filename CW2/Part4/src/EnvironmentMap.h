#ifndef ENVIRONMENTMAP_H
#define ENVIRONMENTMAP_H

#include "LatLong.h"
#include "image.h"

class EnvironmentMap{
public:
	EnvironmentMap(Image* image) : image(image) {}
	~EnvironmentMap() {}

	Vec3f mapTo(float theta, float phi) const{
		unsigned int i = (unsigned int) ((theta/PI)  * ((float) image->height));
		unsigned int j = (unsigned int) ((phi/(2*PI)) * ((float) image->width));
		unsigned int index = (i*image->width + j) * image->numComponents;
		if (index > image->height * image->width * image->numComponents) {
			return Vec3f(1.0f,1.0f,1.0f);
		}
		return Vec3f((*image)[index], (*image)[index+1], (*image)[index+2]);
	}

	Vec3f mapTo(const LatLong& ll) const{
		float theta = ll.theta();
		float phi = ll.phi();
		return this->mapTo(theta, phi);	
	}

	std::vector<Vec3f> mapFrom(vecpairuu samples) {
		std::vector<Vec3f> vectors;
		vectors.reserve(samples.size());

		//Use pixel to work out theta an phi,
		//make a latlong and convert it into a vector
		for (uint i = 0; i < samples.size(); i++) {
			std::pair<ushort,ushort> samp = samples[i];
			uint x = samp.first;
			uint y = samp.second;

			//x = (phi/2PI) * width
			//x/width = phi/2PI
			//(x/width)*2PI = phi
			//y = (theta/PI) * height
			//y/ height = theta/PI
			//theta = (y/height)*PI

			float phi = ((float)x/(float)image->width)*2*PI;
			float theta = ((float)y/(float)image->height)*PI;

			LatLong ll;
			ll.setPhi(phi);
			ll.setTheta(theta);
			Vec3f vec = LatLong::ll2v3f(ll);

//			LatLong ll2 = LatLong::v3f2ll(vec);
//
//			uint y2 = (unsigned int) ((theta/PI)  * ((float) image->height));
//			uint x2 = (unsigned int) ((phi/(2*PI)) * ((float) image->width));

			vectors.push_back(vec);
		}
		return vectors;
	}

private:
	Image* image;
};

#endif
