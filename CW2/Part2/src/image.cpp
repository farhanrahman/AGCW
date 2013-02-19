#include "image.h"
#include <math.h>
#include "colour.h"
#include <fstream>

#define GAMMA 1.3f

#define PI 3.14159265358979323

Image::Image(const char * inputImage){
	this->initialise(inputImage);
}

Image::Image(const char * inputImage, unsigned int exposure){
	this->initialise(inputImage);
	this->exposure = exposure;
}

void Image::initialise(const char * inputImage){
    this->buffer = loadPFM(inputImage, width, height, numComponents);
	for (uint i = 0; i < L; ++i){
		cdf[i] = 0.0;
	}
}

Image::~Image(void){
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

void Image::SetPixel(unsigned int x, unsigned int y, const Vec3f &color){
	if(x < width && y < height){
		unsigned int index = (y*width + x)*numComponents;
		buffer[index] = color.r();
		buffer[index+1] = color.g();
		buffer[index+2] = color.b();
	}
}

void Image::generateCDF(void) {
	uint n;
	uint hist[L] = {0};

	for(uint i = 0; i < height; ++i)
		for(uint j = 0; j < width; ++j){
			uint index = (i*width+j)*numComponents;
			float scale = ((float) i / (float) width) * PI;
			float gray_scale =  sqrt( 0.241*pow(buffer[index],2) + 0.691*pow(buffer[index+1],2) + 0.068*pow(buffer[index+2],2) );
			gray_scale *= (float) (L-1);
			gray_scale *= sin(scale);
			if (gray_scale > L - 1){
				gray_scale = L - 1;
			}
			hist[(uint) gray_scale]++;
	}

	n = width * height;

	cdf[0] = (float) hist[0]/(float)n;;

	for (uint i = 1; i < L; ++i){
		cdf[i] = cdf[i-1] + (float)hist[i]/(float)n;
	}

}




