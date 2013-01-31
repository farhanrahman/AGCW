#include "image.h"

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

float Image::operator[] (unsigned int index){
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
				float res = buffer[index]*255000.0f;
				if (res > 255.0f) {
					res = 255.0f;
				}
//				float res = buffer[index]*255.0f;

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
