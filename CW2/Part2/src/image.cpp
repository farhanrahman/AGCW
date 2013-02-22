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
	cdfX = new float[width];
	cdfY = new float[height];
	for (uint i = 0; i < width; ++i){
		cdfX[i] = 0.0;
	}
	for (uint i = 0; i < height; ++i){
		cdfY[i] = 0.0;
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
	uint freqX[width];
	uint freqY[height];
	float avgX[width];
	float avgY[height];

	for (uint i = 0; i < width; ++i){
		freqX[i] = 0;
		avgX[i] = 0.0;
	}

	for (uint i = 0; i < height; ++i){
		freqY[i] = 0;
		avgY[i] = 0.0;
	}

	/*Generate average energy for columns P(X)*/
	for (uint cols = 0; cols < width; ++cols){
		float sum = 0.0;
		for (uint rows = 0; rows < height; ++rows){
			uint index = (rows*width + cols)*numComponents;
			/*Sum up the luminance values scaled by the sin(theta)*/
			float scale = ((float) rows / (float) width) * PI;
			sum += sin(scale)*sqrt( 0.241*pow(buffer[index],2) + 0.691*pow(buffer[index+1],2) + 0.068*pow(buffer[index+2],2) );
		}
		avgX[cols] = sum / (double) height;
	}

	/*Generate average energy for rows P(Y)*/
	for (uint rows = 0; rows < height; ++rows){
		float sum = 0.0;
		for (uint cols = 0; cols < width; ++cols){
			uint index = (rows*width + cols)*numComponents;
			/*Sum up the luminance values scaled by the sin(theta)*/
			float scale = ((float) rows / (float) width) * PI;
			sum += sin(scale)*sqrt( 0.241*pow(buffer[index],2) + 0.691*pow(buffer[index+1],2) + 0.068*pow(buffer[index+2],2) );
		}
		avgY[rows] = sum / (double) width;
	}

	/*Generate the PDF for P(X)*/
	for (uint i = 0; i < width; ++i){
		float gray_scale = avgX[i]*((double) width - 1);
		if (gray_scale > width - 1)
				gray_scale = width - 1;
		freqX[(uint) gray_scale]++;
	}

	/*Generate the PDF for P(Y)*/
	for (uint i = 0; i < height; ++i){
		float gray_scale = avgY[i]*((double) height - 1);
		if (gray_scale > height - 1)
				gray_scale = height - 1;
		freqY[(uint) gray_scale]++;
	}

	/*Generate the CDFs from the PDFs*/

	cdfX[0] = (float) freqX[0]/(float)width;
	cdfY[0] = (float) freqY[0]/(float)height;
	for (uint i = 1; i < width; ++i){
		cdfX[i] = cdfX[i-1] + (float)freqX[i]/(float)width;
	}

	for (uint i = 1; i < height; ++i){
		cdfY[i] = cdfY[i-1] + (float)freqY[i]/(float)height;
	}

}




