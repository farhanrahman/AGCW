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

void getNormalMap(Image &output, Sphere pSphere);
void writeVectorMap(Image &input, char* filename);

Image getReflectMap(const Image &normalMap);

void renderScene(Image& output, EnvironmentMap& em, Sphere& pSphere);
void renderScene(Image& output, EnvironmentMap& em, Image& normalMap);

int main() {
	//Image worldMap(LL_IMAGE);
	//worldMap.writeToFile("../WorldMap/world.pfm");
	Sphere sphere(Vec3f(0.5f,-0.5f, 100.0f),0.5f);

	Image normalMap(_width,_height);

	Image latLongImg(LL_IMAGE);

	Image output(_width, _height);

	EnvironmentMap envMap(latLongImg);

	getNormalMap(normalMap,sphere);

	writeVectorMap(normalMap, "normalMap.ppm");

	Image reflectMap = getReflectMap(normalMap);

	writeVectorMap(reflectMap, "reflectMap.ppm");

//	renderScene(output, envMap, sphere);
	renderScene(output, envMap, normalMap);

	std::cout << "Finished!" << endl;

	return 0;
}

void getNormalMap(Image &output, Sphere pSphere) {
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

Image getReflectMap(const Image &normalMap) {
	uint height = normalMap.height;
	uint width = normalMap.width;

	Image output = Image(width,height);

	output.SetAllPixels(Vec3f(0.0,0.0,0.0));

	uint numPixels = width*height;

	Vec3f direction(0.0f,0.0f,1.0f);
	direction.Negate();

	for (uint i = 0; i < numPixels; ++i) {
		uint index = i * 3;
		Vec3f normal(normalMap.buffer[index],normalMap.buffer[index+1],normalMap.buffer[index+2]);
		if (normal==Vec3f(0.0f,0.0f,0.0f)) {
			continue;
		}
		float ndotv = normal.Dot3(direction);
		ndotv*=2;
		Vec3f r(normal.Scale(ndotv) - direction);
		r.Normalize();

		output.buffer[index] = r.x();
		output.buffer[index+1] = r.y();
		output.buffer[index+2] = r.z();

	}

	return output;
}

void writeVectorMap(Image &input, char* filename) {
	uint width = input.width;
	uint height = input.height;
	uint channels = input.numComponents;
	Image normalMap = Image(width,height);

	uint arrayLen = height * width * channels;

    for(uint i = 0; i < arrayLen; i++) {
		normalMap.buffer[i] = (input.buffer[i] + 1.0f)/2.0f;
    }

    normalMap.writeAsPPM(filename);
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
			Vec3f intersection;
			if (pSphere.intersect(ray, &intersection)){
				Vec3f sphereNorm = pSphere.getNormalAt(ray);
				sphereNorm.Normalize();

				Vec3f direction = ray.getDirection().Normalized().Negated();
				float ndotv = sphereNorm.Dot3(direction);
				ndotv *= 2.0;

				Vec3f r(sphereNorm.Scale(ndotv) - direction);
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

void renderScene(Image& output, EnvironmentMap& em, Image& normalMap){
	uint height = output.height;
	uint width = output.width;
	output.exposure = 16.0;

	output.SetAllPixels(Vec3f(0.0,0.0,0.0));

	uint numPixels = width*height;

	Vec3f direction(0.0f,0.0f,1.0f);
	direction.Negate();

	for (uint i = 0; i < numPixels; ++i) {
		uint index = i * 3;
		Vec3f normal(normalMap.buffer[index],normalMap.buffer[index+1],normalMap.buffer[index+2]);
		if (normal==Vec3f(0.0f,0.0f,0.0f)) {
			continue;
		}
		float ndotv = normal.Dot3(direction);
		ndotv*=2;
		Vec3f r(normal.Scale(ndotv) - direction);
		LatLong ll(r);
		Vec3f c = em.mapTo(ll);
		output.buffer[index] = c.r();
		output.buffer[index+1] = c.g();
		output.buffer[index+2] = c.b();
	}
	output.writeAsPPMGamma("part2.ppm");
}
