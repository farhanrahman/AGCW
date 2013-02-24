#include "image.h"
#include <math.h>
#include "colour.h"
#include <fstream>
#include <vector>
#include "LatLong.h"

#define GAMMA 1.3f

#define PI 3.14159265358979323

#define CDFRESOLUTION 100000

Image::Image(const char * inputImage){
	this->initialise(inputImage);
}

Image::Image(const char * inputImage, unsigned int exposure){
	this->initialise(inputImage);
	this->exposure = exposure;
}

void Image::initialise(const char * inputImage){
    this->buffer = loadPFM(inputImage, width, height, numComponents);
	cdfX = new uint[CDFRESOLUTION];
	for (uint i = 0; i < CDFRESOLUTION; ++i){
		cdfX[i] = 0.0;
	}
	cdfY = new uint*[width];
	for (uint i = 0; i < width; ++i){
		cdfY[i] = new uint[CDFRESOLUTION];
		for (uint j = 0; j < CDFRESOLUTION; ++j){
			cdfY[i][j] = 0.0;
		}
	}
}

Image::~Image(){
	delete[] buffer;
	if (cdfX != NULL) {
		delete[] cdfX;
	}
	if (cdfY !=NULL) {
		for (uint i = 0; i < width; ++i){
			delete[] cdfY[i];
		}
		delete[] cdfY;
	}
}

float Image::operator[] (unsigned int index) const{
	return (*this).buffer[index];
} 

void Image::writeAsPPM(const char *outputFile){
/*Code implementation also provided in util.cpp, as provided by the coursework stub code*/
	unsigned char *img_out = new unsigned char [width*height*numComponents];

	for ( unsigned int i = 0 ; i < height ; ++i ) // height
	{   
		for ( unsigned int j = 0 ; j < width ; ++j ) // width
		{   
			for ( unsigned int k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
			{   
				unsigned int index = i*width*numComponents + j*numComponents + k; //index within the image

				float res = buffer[index]*255.0f*exposure;
				if (res > 255.0f) {
					res = 255.0f;
				}

				img_out[index] = (unsigned char) res; //R
			}   
		}   
	}   

	WritePNM(outputFile, width, height, numComponents, img_out);
	delete img_out;
}

void Image::writeAsPPMGamma(const char *outputFile){
	unsigned char *img_out = new unsigned char [width*height*numComponents];

	for ( unsigned int i = 0 ; i < height ; ++i ) // height
	{
		for ( unsigned int j = 0 ; j < width ; ++j ) // width
		{
			for ( unsigned int k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
			{
				unsigned int index = i*width*numComponents + j*numComponents + k; //index within the image

				/*typecast 0.0f -> 1.0f values to the 0 - 255 range*/
				float res = pow(buffer[index]*255.0f * exposure, 1/GAMMA);
				if (res > 255.0f) {
					res = 255.0f;
				}

				img_out[index] = (unsigned char) res; //R
			}
		}
	}

	WritePNM(outputFile, width, height, numComponents, img_out);
	delete img_out;
}


void Image::writeToFile(const char *outputFile){
	WritePFM(outputFile, width, height, numComponents, this->buffer);
}


void Image::SetAllPixels(const Vec3f &color){
	for (unsigned int i = 0; i < height; ++i)
		for (unsigned int j = 0; j < width; ++j){
			unsigned int index = (i*width + j)*numComponents;			
			buffer[index] = color.r();
			buffer[index+1] = color.g();
			buffer[index+2] = color.b(); 
	}
}

void Image::SetPixel(unsigned int x, unsigned int y, Vec3f const &color){
	if(x < width && y < height){
		unsigned int index = (y*width + x)*numComponents;
		buffer[index] = color.r();
		buffer[index+1] = color.g();
		buffer[index+2] = color.b();
	}
}

void Image::generateCDF(void) {
	{
		double avgX[width];

		double cumsum = 0;

		/*Generate average energy for columns P(X)*/
		for (uint cols = 0; cols < width; ++cols){
			double sum = 0.0;
			for (uint rows = 0; rows < height; ++rows){
				uint index = (rows*width + cols)*numComponents;
				/*Sum up the luminance values scaled by the sin(theta)*/
				float scale = ((float) rows / (float) width) * PI;
				float average = (buffer[index] + buffer[index+1] + buffer[index+2])/3;
				float lum = sin(scale)*average;
				sum += sin(scale)*lum;
			}
			cumsum+=sum;
			avgX[cols] = sum;
		}

		const double lumPerIndex = cumsum / (double)CDFRESOLUTION;
		double index = 0;
		uint flrIndex = 0;
		for (uint i = 0; i < width; i++) {
			float val = avgX[i];
			double numInds = val / lumPerIndex;
			index+=numInds;
			uint flrNextIndex = floor(index);
			uint delta = flrNextIndex - flrIndex;
			for (uint j = 0; j < delta; j++) {
				cdfX[flrIndex] = i;
				flrIndex++;
			}
		}
	}
	/*Now do the same for EACH AND EVERY COLUMN Y*/

	for (uint cols = 0; cols < width; cols++) {

		double avgY[height];

		double cumsum = 0;
		/*Generate average energy for columns P(Y)*/
		for (uint rows = 0; rows < height; ++rows){
			uint index = (rows*width + cols)*numComponents;
			/*Sum up the luminance values scaled by the sin(theta)*/
			float scale = ((float) rows / (float) width) * PI;
			float average = (buffer[index] + buffer[index+1] + buffer[index+2])/3;
			float lum = sin(scale)*average;

			cumsum+=lum;
			avgY[rows] = lum;
		}

		const double lumPerIndex = cumsum / (double)CDFRESOLUTION;
		double index = 0;
		uint flrIndex = 0;
		for (uint i = 0; i < height; i++) {
			float val = avgY[i];
			double numInds = val / lumPerIndex;
			index+=numInds;
			uint flrNextIndex = floor(index);
			uint delta = flrNextIndex - flrIndex;
			for (uint j = 0; j < delta; j++) {
				cdfY[cols][flrIndex] = i;
				flrIndex++;
			}
		}
	}

}

vecpairuu Image::getEnvMapSamplesFromCDF(uint numSamples) {
	vecpairuu samples;
	samples.reserve(numSamples);

	srand(0);
	for (uint i = 0; i < numSamples; i++) {
		uint randx = rand()%CDFRESOLUTION;
		uint column = cdfX[randx];
		uint randy = rand()%CDFRESOLUTION;
		uint row = cdfY[column][randy];
		samples.push_back(std::pair<uint,uint>(column,row));
	}
	return samples;
}

void Image::highlightSamples(vecpairuu samples) {
	const Vec3f green = Vec3f(0.0f,1.0f,0.0f);
	for (uint i = 0; i < samples.size(); i++) {
		std::pair<uint,uint> pix = samples[i];
		for (int j = -2; j <=2; j++) {
			for (int k = -2; k <=2; k++) {
				SetPixel(pix.first+j,pix.second+k,green);
			}
		}
	}
}

struct latlong{
	float theta;
	float phi;
	latlong(float theta, float phi) : theta(theta), phi(phi){}
};

latlong Image::phong(float n, float u1, float u2){
	return latlong(acos(pow(1.0-u1, 1.0/(n+1.0))), 2.0*PI*u2);
}

void Image::generateSamplesFromPhongBRDF(float exponent, uint sampleNum){
	float n = exponent;
	vecpairuu samples;
	for (uint i = 0; i < sampleNum; ++i){
		float a = (float) rand()/(float) RAND_MAX;
		float b = (float) rand()/(float) RAND_MAX;
		latlong ll(phong(n, a, b));
		int x = (int) ((ll.phi/(2.0*PI)) * width);
		int y = (int) ((ll.theta/PI) * height);
		samples.push_back(std::pair<uint,uint>(x,y));
	}

	this->highlightSamples(samples);

}

Vec3f Image::at(uint x, uint y) {
	uint index = ((y * width) + x)*numComponents;
	return Vec3f(buffer[index],buffer[index+1],buffer[index+2]);
}



