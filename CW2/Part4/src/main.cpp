//============================================================================
// Name        : CW2Part2.cpp
//============================================================================

#include <iostream>
#include "image.h"
#include <sstream>
#include "ray.h"
#include "sphere.h"
#include "EnvironmentMap.h"

/*For some reason it needs the full path. Change it to
 *your own directory if you have to compile it on your
 *own machine.*/
#ifdef __CYGWIN__
#define LL_IMAGE "../media/grace_latlong.pfm"
#else
#define LL_IMAGE "/home/fr909/git/AGCW/CW2/Part2/GraceCathedral/grace_latlong.pfm"
#endif

typedef unsigned int uint;

uint _width = 511;
uint _height = 511;

void getNormalMap(Image &output, Sphere pSphere);
void renderDiffuse(Image& output, Image& em, Image& normalMap,
		 vecpairuu& samples, std::vector<Vec3f>& normals);

Image getReflectMap(const Image &normalMap);


int main(int argc, char * argv[]) {

	Image latLongImg(LL_IMAGE);

	EnvironmentMap envMap(&latLongImg);

	latLongImg.generateCDF();

	Sphere sphere(Vec3f(-0.5f,-0.5f, 100.0f),0.5f);

	Image normalMap(_width,_height);

	getNormalMap(normalMap,sphere);

//	Image reflectMap = getReflectMap(normalMap);

	uint numSamps[] = {64,256,1024};

	for (uint i = 0; i < sizeof(numSamps)/sizeof(uint); i++) {
		vecpairuu samples = latLongImg.getEnvMapSamplesFromCDF(numSamps[i]);

		std::vector<Vec3f> sampnormals = envMap.mapFrom(samples);

		Image output(_width, _height);

		renderDiffuse(output,latLongImg,normalMap,samples,sampnormals);

		output.exposure = 16;

		stringstream ppm;
		ppm << "sphere" << numSamps[i] << ".ppm";
		output.writeAsPPMGamma(ppm.str().c_str());

		stringstream pfm;
		pfm << "sphere" << numSamps[i] << ".pfm";
		output.writeToFile(pfm.str().c_str());

//		Image envsamp = Image(LL_IMAGE);
//		envsamp.exposure = 10;
//		envsamp.highlightSamples(samples);
//
//		stringstream ppm2;
//		ppm2 << "env" << numSamps[i] << ".ppm";
//		envsamp.writeAsPPM(ppm2.str().c_str());
//
//		stringstream pfm2;
//		pfm2 << "env" << numSamps[i] << ".pfm";
//		envsamp.writeToFile(pfm2.str().c_str());
	}


	return 0;
}

void getNormalMap(Image &output, Sphere pSphere) {
	uint height = output.height;
	uint width = output.width;
    for(uint i = 0; i < height; i++) {
        for(uint j = 0; j < width; j++){
        	float origY = (float) i/height;
        	float origX = (float) j/width;
        	Ray incident = Ray(Vec3f(0.0f,0.0f,1.0f),Vec3f(-origX,-origY,0.0f));
			Vec3f normal = pSphere.getNormalAt(incident);

			output.SetPixel(j,i,normal);
        }
    }
}

void renderDiffuse(Image& output, Image& em, Image& normalMap,
		 vecpairuu& samples, std::vector<Vec3f>& normals){
	uint height = output.height;
	uint width = output.width;
	output.exposure = 16.0;

	output.SetAllPixels(Vec3f(0.0,0.0,0.0));
	uint numPixels = width*height;
	uint numSamples = samples.size();

	for (uint i = 0; i < numPixels; ++i) {
		uint index = i * 3;
		Vec3f normal(normalMap.buffer[index],normalMap.buffer[index+1],normalMap.buffer[index+2]);
		Vec3f result(0,0,0);
		if (normal != Vec3f(0,0,0)) {
			for (uint j = 0; j < numSamples; ++j) {
				uint x = samples[j].first;
				uint y = samples[j].second;
				Vec3f normal2 = normals[j];
				Vec3f colour(em.at(x,y));
				float colsum = colour.r() + colour.g() + colour.b();
				colour.Divide(colsum,colsum,colsum);
				float intensity = max(0.0f,normal.Dot3(normal2));
				result+=colour*intensity;
			}
			result.Divide(numSamples,numSamples,numSamples);
		}
		output.buffer[index] = result.r();
		output.buffer[index+1] = result.g();
		output.buffer[index+2] = result.b();
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

