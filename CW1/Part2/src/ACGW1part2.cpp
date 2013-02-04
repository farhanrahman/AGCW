#include <iostream>
#include "image.h"
#include "EnvironmentMap.h"
#include "ray.h"
#include "sphere.h"

#define LL_IMAGE "../GraceCathedral/grace_latlong.pfm"

#define UINT unsigned int

UINT _width = 511;
UINT _height = 511;

void getNormalMap(Image output, Sphere pSphere);
void writeNormalMap(Image input);

int main() {

	Sphere sphere(Vec3f(0.5f,0.5f,0.0f),0.5f);

	Image normalMap(_width,_height);

	Image latLongImg(LL_IMAGE);

	EnvironmentMap envMap(latLongImg);

	getNormalMap(normalMap,sphere);

	writeNormalMap(normalMap);

	std::cout << "Finished!" << endl;

	return 0;
}

void getNormalMap(Image output, Sphere pSphere) {
	uint height = output.height;
	uint width = output.width;
    for(uint i = 0; i < height; i++) {
        for(uint j = 0; j < width; j++){
        	float origY = (float) i/height;
        	float origX = (float) j/height;
        	Ray incident = Ray(Vec3f(0.0f,0.0f,1.0f),Vec3f(origX,origY,0.0f));
			Vec3f normal = pSphere.getNormalAt(incident);

			output.SetPixel(j,i,normal);
        }
    }
}

void writeNormalMap(Image input) {
	uint width = input.width;
	uint height = input.height;
	uint channels = input.numComponents;
	Image normalMap = Image(width,height);

	uint arrayLen = height * width * channels;

    for(uint i = 0; i < arrayLen; i++) {
		normalMap.buffer[i] = (input.buffer[i] + 1.0f)/2.0f;
    }

    normalMap.writeAsPPM("normalMap.ppm");
}
