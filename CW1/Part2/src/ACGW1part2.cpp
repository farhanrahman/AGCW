#include <iostream>
#include "image.h"
#include "EnvironmentMap.h"
#include "ray.h"
#include "sphere.h"
#include <limits>
#define LL_IMAGE "../GraceCathedral/grace_latlong.pfm"
//#define LL_IMAGE "../WorldMap/world.pfm"
#define UINT unsigned int

UINT _width = 511;
UINT _height = 511;

void getNormalMap(Image output, Sphere pSphere);
void writeNormalMap(Image input);
void renderScene(Image& output, EnvironmentMap& em, Sphere& pSphere);

int main() {
	//Image worldMap(LL_IMAGE);
	//worldMap.writeToFile("../WorldMap/world.pfm");
	Sphere sphere(Vec3f(0.5f,-0.5f, 20000.0f),0.5f);

	Image normalMap(_width,_height);

	Image latLongImg(LL_IMAGE);

	Image output(_width, _height);

	EnvironmentMap envMap(latLongImg);

	getNormalMap(normalMap,sphere);

	writeNormalMap(normalMap);

	renderScene(output, envMap, sphere);

	std::cout << "Finished!" << endl;

	return 0;
}

void getNormalMap(Image output, Sphere pSphere) {
	uint height = output.height;
	uint width = output.width;
    for(uint i = 0; i < height; i++) {
        for(uint j = 0; j < width; j++){
        	float origY = (float) i/height;
        	float origX = (float) j/width;
        	Ray incident = Ray(Vec3f(0.0f,0.0f,1.0f),Vec3f(origX,-origY,0.0f));
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

void renderScene(Image& output, EnvironmentMap& em, Sphere& pSphere){
	uint height = output.height;
	uint width = output.width;
	output.exposure = 16.0;
	output.SetAllPixels(Vec3f(0.0,0.0,0.0));
	for (uint i = 0; i < height; ++i)
		for(uint j = 0; j < width; ++j){
			float x = (float) i/height;
			float y = (float) j/width;
			Ray ray = Ray(Vec3f(0.0,0.0,1.0), Vec3f(x,-y,0.0));
			Vec3f v;
			if (pSphere.intersect(ray, &v)){
				//v = ray.getDirection();//+ray.getOrigin();
				Vec3f n = pSphere.getNormalAt(ray);
				n.Normalize();
				v.Normalize();
				v.Negate();
				float ndotv = n.Dot3(v);
				ndotv *= 2.0;
				Vec3f r(n.Scale(ndotv) - v);
				LatLong ll(r);
				Vec3f c = em.mapTo(ll);
				uint index = (uint) (i*width+j)*output.numComponents;
				output.buffer[index] = c.r();
				output.buffer[index+1] = c.g();
				output.buffer[index+2] = c.b();
			}
	}
	output.writeAsPPMGamma("part2.ppm");
}
