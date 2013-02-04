#include "image.h"
#include <math.h>
#include "colour.h"
#include "convUtil.h"
#include <fstream>

#define GAMMA 1.2f

Image::Image(const char * inputImage){
	this->initialise(inputImage);
}

Image::Image(const char * inputImage, unsigned int exposure){
	this->initialise(inputImage);
	this->exposure = exposure;
}

void Image::initialise(const char * inputImage){

    this->buffer = loadPFM(inputImage, width, height, numComponents);
}

Image::~Image(void){
	//delete buffer;
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

//				typecast 0.0f -> 1.0f values to the 0 - 255 range
//				float res = pow(buffer[index]*255.0f * exposure, 1/GAMMA);
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

#define RESOLUTION 100000

void Image::writeAsHDRPPM(const char *outputFile){
/*Code implementation also provided in util.cpp, as provided by the coursework stub code*/
	unsigned char *img_out = new unsigned char [width*height*numComponents];

	unsigned int numPixels = width*height;
	unsigned int freq[RESOLUTION] = {};
	float tonemap[RESOLUTION];

	for ( unsigned int i = 0 ; i < height ; ++i ) // height
	{
		for ( unsigned int j = 0 ; j < width ; ++j ) // width
		{
			unsigned int pi = (i*width+j)*numComponents;
			float average = (0.2126 * buffer[pi] + 0.7152 * buffer[pi+1] + 0.0722 * buffer[pi+2]);
			float ind = average * (RESOLUTION-1);
			freq[(int) ind]++;
		}
	}

	for (int i = 0; i < RESOLUTION; i++) {

		float frequency = freq[i];

		if (i==0) {
			if (freq[i] == 0) {
				tonemap[i] = 0;
			}
			else {
				tonemap[i] = (frequency / numPixels);
			}
		}
		else if (freq[i] == 0) {
			tonemap[i] = tonemap[i-1];
		}
		else {
			tonemap[i] = (frequency / numPixels) + tonemap[i-1];
		}

	}

	for ( unsigned int i = 0 ; i < height ; ++i ) // height
	{
		for ( unsigned int j = 0 ; j < width ; ++j ) // width
		{
//			unsigned int pi = (i*width+j)*numComponents;
//			float average = ((float)(RESOLUTION-1)) * (0.2126 * buffer[pi] + 0.7152 * buffer[pi+1] + 0.0722 * buffer[pi+2]);
//
//			float toned = tonemap[(int)average];
			for ( unsigned int k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
			{
				unsigned int index = i*width*numComponents + j*numComponents + k; //index within the image

				int channel = buffer[index] * (RESOLUTION-1);

				float res = tonemap[channel] * 255;
//				float res = toned * buffer[index] * 255 * 255;

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

#define L 100000

void Image::histogramEqualization(unsigned char **imgPP, uint px, uint py, uint windowsize, uint iw, uint ih){
	int startX, startY;
	int endX, endY;
	unsigned char *img_out = (*imgPP);
	float cdf[L] = {0.0};
	uint n;
	uint hist[L] = {0.0};

	int hw = floor(windowsize/2);

	if (px < hw) {
		startX = ((int) px) - hw;
	} else {
		startX = px - hw;
	}

	if (py < hw){
		startY = ((int) py) - hw;
	} else {
		startY = py - hw;
	}

	endX = px + hw;
	endY = py + hw;

	for (int i = startY; i <= endY; ++i)
		for(int j = startX; j <= endX; ++j){
			uint tempX, tempY;
			if (j < 0) {
				tempX = (uint) (-1 * j);
			} else if (j > iw){
				tempX = (uint) (iw - (j - iw));
			} else {
				tempX = j;
			}

			if (i < 0) {
				tempY = (uint) (-1 * i);
			} else if (i > ih){
				tempY = (uint) (ih - (i - ih));
			} else {
				tempY = i;
			}

			
                        uint index = (tempY*iw+tempX)*numComponents;
                        float gray_scale = (0.2126 * buffer[index] + 0.7152 * buffer[index+1] + 0.0722 * buffer[index+2]);
			gray_scale *= (float) (L-1);
                        hist[(uint) gray_scale]++;
	}
	
	n = windowsize * windowsize;



	cdf[0] = (float) hist[0] / (float) n;

	for (uint i = 1; i < L; ++i){
		cdf[i] = cdf[i-1] + (float) hist[i] / (float) n;
	}

	for (int k = 0; k < numComponents; ++k){
		uint pi = (py*iw+px)*numComponents + k;	
		float hi = buffer[pi]*(L-1);
		float res = (float) cdf[(uint) hi] * 255.0f;
		if (res > 255.0f){
			res = 255.0f;
		}
		img_out[pi] = (unsigned char) (res);
	}

}

#define W_SIZE 50
#define ADAPTIVE 0

void Image::writeAsHDRPPM2(const char *outputFile){	
	unsigned char *img_out = new unsigned char [width*height*numComponents];
	#if ADAPTIVE == 0
	float cdf[L]={0.0};
	uint n;
	uint hist[L] = {0};
	
	for(uint i = 0; i < height; ++i)
		for(uint j = 0; j < width; ++j){
			uint index = (i*width+j)*numComponents;
			float gray_scale =  sqrt( 0.241*pow(buffer[index],2) + 0.691*pow(buffer[index+1],2) + 0.068*pow(buffer[index+2],2) );
			gray_scale *= (float) (L-1);
			hist[(uint) gray_scale]++;
	}



	n = width * height;

	cdf[0] = (float) hist[0]/(float)n;;
	
	for (uint i = 1; i < L; ++i){
		cdf[i] = cdf[i-1] + (float)hist[i]/(float)n;
	}	

	for(uint i = 0; i < height; ++i)
		for(uint j = 0; j < width; ++j)
			for(uint k = 0; k < numComponents; ++k){
			uint index = (i*width+j)*numComponents + k;
	 		float hi = buffer[index]*(L-1);
			float res = (float) cdf[(uint) hi] * 255.0f;
			if (res > 255.0f){
				res = 255.0f;
			}
	 		img_out[index] = (unsigned char)(res);
	}

	#else
	for(uint i = 0; i < height; ++i)
		for(uint j = 0; j < width; ++j){
			histogramEqualization(&img_out, i, j, W_SIZE, (*this).width, (*this).height);		
	}
	#endif
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
	if(x >= 0 && x < width && y >= 0 && y < height){
		unsigned int index = (x*width + y)*numComponents;
		buffer[index] = color.r();
		buffer[index+1] = color.g();
		buffer[index+2] = color.b();
	}
}




